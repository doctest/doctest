#ifndef DOCTEST_MPI_REPORTER_H
#define DOCTEST_MPI_REPORTER_H

// #include <doctest/doctest.h>
#include <fstream>
#include <string>
#include "mpi.h"


#include <vector>
#include <mutex>

namespace doctest {

extern int nb_test_cases_skipped_insufficient_procs;
int mpi_comm_world_size();

namespace {

// https://stackoverflow.com/a/11826666/1583122
struct NullBuffer : std::streambuf {
  int overflow(int c) { return c; }
};
class NullStream : public std::ostream {
  public:
    NullStream()
      : std::ostream(&nullBuff)
    {}
  private:
    NullBuffer nullBuff = {};
};
static NullStream nullStream;


/* \brief Extends the ConsoleReporter of doctest
 *        Each process writes its results to its own file
 *        Intended to be used when a test assertion fails and the user wants to know exactly what happens on which process
 */
struct MpiFileReporter : public ConsoleReporter {
  std::ofstream logfile_stream = {};

  MpiFileReporter(const ContextOptions& co)
    : ConsoleReporter(co,logfile_stream)
  {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string logfile_name = "doctest_" + std::to_string(rank) + ".log";

    logfile_stream = std::ofstream(logfile_name.c_str(), std::fstream::out);
  }
};


/* \brief Extends the ConsoleReporter of doctest
 *        Allows to manage the execution of tests in a parallel framework
 *        All results are collected on rank 0
 */
struct MpiConsoleReporter : public ConsoleReporter {
private:
  static std::ostream& replace_by_null_if_not_rank_0(std::ostream* os) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
      return *os;
    } else {
      return nullStream;
    }
  }
  std::vector<std::pair<std::string, int>> m_failure_str_queue = {};
public:
  MpiConsoleReporter(const ContextOptions& co)
    : ConsoleReporter(co,replace_by_null_if_not_rank_0(co.cout))
  {}

  std::string file_line_to_string(const char* file, int line,
                                  const char* tail = ""){
    std::stringstream ss;
    ss << skipPathFromFilename(file)
    << (opt.gnu_file_line ? ":" : "(")
    << (opt.no_line_numbers ? 0 : line) // 0 or the real num depending on the option
    << (opt.gnu_file_line ? ":" : "):") << tail;
    return ss.str();
  }

  void test_run_end(const TestRunStats& p) override {
    ConsoleReporter::test_run_end(p);

    const bool anythingFailed = p.numTestCasesFailed > 0 || p.numAssertsFailed > 0;

    // -----------------------------------------------------
    // > Gather information in rank 0
    int n_rank, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);

    int g_numAsserts         = 0;
    int g_numAssertsFailed   = 0;
    int g_numTestCasesFailed = 0;

    MPI_Reduce(&p.numAsserts        , &g_numAsserts        , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p.numAssertsFailed  , &g_numAssertsFailed  , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p.numTestCasesFailed, &g_numTestCasesFailed, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    std::vector<int> numAssertsFailedByRank;
    if(rank == 0){
      numAssertsFailedByRank.resize(static_cast<std::size_t>(n_rank));
    }

    MPI_Gather(&p.numAssertsFailed, 1, MPI_INT, numAssertsFailedByRank.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
      separator_to_stream();
      s << Color::Cyan << "[doctest] " << Color::None << "assertions on all processes: " << std::setw(6)
        << g_numAsserts << " | "
        << ((g_numAsserts == 0 || anythingFailed) ? Color::None : Color::Green)
        << std::setw(6) << (g_numAsserts - g_numAssertsFailed) << " passed" << Color::None
        << " | " << (g_numAssertsFailed > 0 ? Color::Red : Color::None) << std::setw(6)
        << g_numAssertsFailed << " failed" << Color::None << " |\n";
      if (nb_test_cases_skipped_insufficient_procs>0) {
        s << Color::Cyan << "[doctest] " << Color::Yellow << "WARNING: Skipped ";
        if (nb_test_cases_skipped_insufficient_procs>1) {
          s << nb_test_cases_skipped_insufficient_procs << " tests requiring more than ";
        } else {
          s << nb_test_cases_skipped_insufficient_procs << " test requiring more than ";
        }
        if (mpi_comm_world_size()>1) {
          s << mpi_comm_world_size() << " MPI processes to run\n";
        } else {
          s << mpi_comm_world_size() << " MPI process to run\n";
        }
      }

      separator_to_stream();
      if(g_numAssertsFailed > 0){

        s << Color::Cyan << "[doctest] " << Color::None << "fail on rank:" << std::setw(6) << "\n";
        for(std::size_t i = 0; i < numAssertsFailedByRank.size(); ++i){
          if( numAssertsFailedByRank[i] > 0 ){
            s << std::setw(16) << " -> On rank [" << i << "] with " << numAssertsFailedByRank[i] << " test failed" << std::endl;
          }
        }
      }
      s << Color::Cyan << "[doctest] " << Color::None
        << "Status: " << (g_numTestCasesFailed > 0 ? Color::Red : Color::Green)
        << ((g_numTestCasesFailed > 0) ? "FAILURE!" : "SUCCESS!") << Color::None << std::endl;
    }
  }

  void test_case_end(const CurrentTestCaseStats& st) override {
    if (is_mpi_test_case()) {
      // function called by every rank at the end of a test
      // if failed assertions happened, they have been sent to rank 0
      // here rank zero gathers them and prints them all

      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      std::vector<MPI_Request> requests;
      requests.reserve(m_failure_str_queue.size());  // avoid realloc & copy of MPI_Request
      for (const std::pair<std::string, int> &failure : m_failure_str_queue)
      {
        const std::string & failure_str = failure.first;
        const int failure_line = failure.second;

        int failure_msg_size = static_cast<int>(failure_str.size());

        requests.push_back(MPI_REQUEST_NULL);
        MPI_Isend(failure_str.c_str(), failure_msg_size, MPI_BYTE,
                 0, failure_line, MPI_COMM_WORLD, &requests.back()); // Tag = file line
      }


      // Compute the number of assert with fail among all procs
      const int nb_fail_asserts = static_cast<int>(m_failure_str_queue.size());
      int nb_fail_asserts_glob = 0;
      MPI_Reduce(&nb_fail_asserts, &nb_fail_asserts_glob, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

      if(rank == 0) {
        MPI_Status status;
        MPI_Status status_recv;

        using id_string = std::pair<int,std::string>;
        std::vector<id_string> msgs(static_cast<std::size_t>(nb_fail_asserts_glob));

        for (std::size_t i=0; i<static_cast<std::size_t>(nb_fail_asserts_glob); ++i) {
          MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

          int count;
          MPI_Get_count(&status, MPI_BYTE, &count);

          std::string recv_msg(static_cast<std::size_t>(count),'\0');
          void* recv_msg_data = const_cast<char*>(recv_msg.data()); // const_cast needed. Non-const .data() exists in C++11 though...
          MPI_Recv(recv_msg_data, count, MPI_BYTE, status.MPI_SOURCE,
                   status.MPI_TAG, MPI_COMM_WORLD, &status_recv);

          msgs[i] = {status.MPI_SOURCE,recv_msg};
        }

        std::sort(begin(msgs),end(msgs),[](const id_string& x, const id_string& y){ return x.first < y.first; });

        // print
        if (nb_fail_asserts_glob>0) {
          separator_to_stream();
          file_line_to_stream(tc->m_file.c_str(), static_cast<int>(tc->m_line), "\n");
          if(tc->m_test_suite && tc->m_test_suite[0] != '\0')
            s << Color::Yellow << "TEST SUITE: " << Color::None << tc->m_test_suite << "\n";
          if(strncmp(tc->m_name, "  Scenario:", 11) != 0)
            s << Color::Yellow << "TEST CASE:  ";
          s << Color::None << tc->m_name << "\n\n";
          for(const auto& msg : msgs) {
            s << msg.second;
          }
          s << "\n";
        }
      }

      MPI_Waitall(static_cast<int>(requests.size()), requests.data(), MPI_STATUSES_IGNORE);
      m_failure_str_queue.clear();
    }

    ConsoleReporter::test_case_end(st);
  }

  bool is_mpi_test_case() const {
    return tc->m_description != nullptr
        && std::string(tc->m_description) == std::string("MPI_TEST_CASE");
  }

  void log_assert(const AssertData& rb) override {
    if (!is_mpi_test_case()) {
      ConsoleReporter::log_assert(rb);
    } else {
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);


      if(!rb.m_failed && !opt.success)
        return;

      std::lock_guard<std::mutex> lock(mutex);

      std::stringstream failure_msg;
      failure_msg << Color::Red << "On rank [" << rank << "] : " << Color::None;
      failure_msg << file_line_to_string(rb.m_file, rb.m_line, " ");

      if((rb.m_at & (assertType::is_throws_as | assertType::is_throws_with)) ==0){
        failure_msg << Color::Cyan
                    << assertString(rb.m_at)
                    << "( " << rb.m_expr << " ) "
                    << Color::None

                    << (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n")
                    << "  values: "
                    << assertString(rb.m_at)
                    << "( " << rb.m_decomp.c_str() << " )\n";
      }

      m_failure_str_queue.push_back({failure_msg.str(), rb.m_line});
    }
  }
}; // MpiConsoleReporter

// "1" is the priority - used for ordering when multiple reporters/listeners are used
REGISTER_REPORTER("MpiConsoleReporter", 1, MpiConsoleReporter);
REGISTER_REPORTER("MpiFileReporter", 1, MpiFileReporter);

} // anonymous
} // doctest

#endif // DOCTEST_REPORTER_H

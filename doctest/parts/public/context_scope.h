namespace doctest {

struct DOCTEST_INTERFACE IContextScope
{
    DOCTEST_DECLARE_INTERFACE(IContextScope)
    virtual void stringify(std::ostream*) const = 0;
};

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {

  // ContextScope base class used to allow implementing methods of ContextScope
  // that don't depend on the template parameter in doctest.cpp.
  struct DOCTEST_INTERFACE ContextScopeBase : public IContextScope {
      ContextScopeBase(const ContextScopeBase&) = delete;

      ContextScopeBase& operator=(const ContextScopeBase&) = delete;
      ContextScopeBase& operator=(ContextScopeBase&&) = delete;

      ~ContextScopeBase() override = default;

  protected:
      ContextScopeBase();
      ContextScopeBase(ContextScopeBase&& other) noexcept;

      void destroy();
      bool need_to_destroy{true};
  };

  template <typename L> class ContextScope : public ContextScopeBase
  {
      L lambda_;

  public:
      explicit ContextScope(const L &lambda) : lambda_(lambda) {}
      explicit ContextScope(L&& lambda) : lambda_(static_cast<L&&>(lambda)) { }

      ContextScope(const ContextScope&) = delete;
      ContextScope(ContextScope&&) noexcept = default;

      ContextScope& operator=(const ContextScope&) = delete;
      ContextScope& operator=(ContextScope&&) = delete;

      void stringify(std::ostream* s) const override { lambda_(s); }

      ~ContextScope() override {
          if (need_to_destroy) {
              destroy();
          }
      }
  };

  template <typename L>
  ContextScope<L> MakeContextScope(const L &lambda) {
      return ContextScope<L>(lambda);
  }

} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest

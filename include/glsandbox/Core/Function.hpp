#ifndef FUNCTION_HPP_
#define FUNCTION_HPP_

template <typename ReturnType, typename... Args> class Function {
  using FunctionPointerType = ReturnType (*)(Args...);

public:
  Function() = delete;
  Function(FunctionPointerType functionPointer)
      : m_FunctionPointer(functionPointer) {}
  Function(Function &function)
      : m_FunctionPointer(function.m_FunctionPointer) {}
  Function(Function &&function) noexcept
      : m_FunctionPointer(function.m_FunctionPointer) {}

  virtual ~Function() {}

  inline void Set(FunctionPointerType functionPointer) {
    m_FunctionPointer = functionPointer;
  }

  inline ReturnType Call(Args... args) const {
    return m_FunctionPointer(args...);
  }

  ReturnType operator()(Args... args) const { Call(args...); }

  Function &operator=(const Function &function) { Set(function); }

  Function &operator=(Function &&function) noexcept { Set(function); }

  Function &operator=(const FunctionPointerType &functionPointer) {
    Set(functionPointer);
  }
  Function &operator=(FunctionPointerType &&functionPointer) noexcept {
    Set(functionPointer);
  }

private:
  FunctionPointerType m_FunctionPointer;
};

template <typename... Args> using Proc = Function<void, Args...>;

#endif // FUNCTION_HPP_

#pragma once
#include <vector>
#include <memory>
#include "Metadata.h"
#include "MetaValue.h"

class METADATA_API CFunction : public CMetadata
{
public:
    CFunction(const std::string& name, void* ptr)
        : CMetadata(name)
        , Ptr(ptr)
    {}

    struct CFrame
    {
        CFrame(){}
        virtual ~CFrame() = default;
        CFrame(const CFrame&) = delete;
        CFrame(CFrame&&) = delete;
        CFrame& operator=(const CFrame&) = delete;
        CFrame& operator=(CFrame&&) = delete;
        virtual void* GetRetTuple() = 0;
        virtual void* GetArgsTuple() = 0;

        virtual void* GetRet() = 0;
        virtual void* GetArg(uint32_t index) = 0;
    };

    virtual void Invoke(CFrame* Frame) = 0;
    virtual std::unique_ptr<CFrame> NewFrame() = 0;

protected:
    void* Ptr;
    CParameter* Return;
    std::vector<CParameter*> Params;
};

template <typename Ret, typename... Args>
class TCppFunction : public CFunction
{
private:
    using CFunctionPtr = Ret(*)(Args...);
public:
    struct CFrame : public CFunction::CFrame
    {
    private:

        template<size_t N>
        void SaveArgsAddress()
        {
            //ArgsPtr[N] = reinterpret_cast<void*>(&std::get<N>(ArgsTuple));
            using ArgNType = decltype(std::get<N>(ArgsTuple));
            if constexpr (!std::is_pointer_v<ArgNType> && !std::is_reference_v<ArgNType>)
            {
                ArgsPtr[N] = new ArgNType();
            }
            else
            {
                ArgsPtr[N] = reinterpret_cast<void*>(&std::get<N>(ArgsTuple));
            }
            //if constexpr (std::is_pointer_v<> && std::is_reference_v<decltype(std::get<N>(ArgsTuple))>)
            //{
            //    ArgsPtr[N] = reinterpret_cast<void*>(&std::get<N>(ArgsTuple));

            //    //ArgsPtr[N] = new (std::remove_cv_t<std::remove_reference_t<decltype(std::get<N>(ArgsTuple))>>*)();
            //}
            //else
            //{
            //    ArgsPtr[N] = reinterpret_cast<void*>(&std::get<N>(ArgsTuple));
            //}
            if constexpr (N > 0) return SaveArgsAddress<N - 1>();
        }

        //template <size_t N,  size_t Indices...>
        //void  InvokeImpl(N, std::index_sequence<indices...> indexSequence) const
        //{
        //    ArgsPtr[Indices].
        //}

        template<typename T>
        struct TArgument
        {};

        template<typename T>
        void NewArgsBuffers(size_t index, TArgument<T> argument)
        {
            if constexpr (!std::is_pointer_v<T> && !std::is_reference_v<T>)
            {
                ArgsPtr[index] = new T();
            }
        }

        template<typename T, typename ...Arguments>
        void NewArgsBuffers(size_t index, TArgument<T> argument, TArgument<Arguments>... arguments)
        {
            if constexpr (!std::is_pointer_v<T> && !std::is_reference_v<T>)
            {
                ArgsPtr[index] = new T();
            }
            //else
            //{
            //    ArgsPtr[N] = reinterpret_cast<void*>(&std::get<N>(ArgsTuple));
            //}
            NewArgsBuffers(index + 1, arguments...);
        }

    public:
        CFrame()
            : CFunction::CFrame()
        {
            NewArgsBuffers(0, TArgument<Args>()...);
            if constexpr (!std::is_pointer_v<Ret> && !std::is_reference_v<Ret>)
            {
                RetPtr = new Ret();
            }
        }

        virtual void* GetRetTuple() override { return &RetTuple; }
        virtual void* GetArgsTuple() override { return &ArgsTuple; }
        virtual void* GetRet() override { return RetPtr; }
        virtual void* GetArg(uint32_t index) override { return ArgsPtr[index]; }

    private:
        std::tuple<Ret> RetTuple;
        std::tuple<Args...> ArgsTuple;
        void* RetPtr;
        void* ArgsPtr[sizeof...(Args)];
    };

    virtual void Invoke(CFunction::CFrame* Frame) override
    {
        std::tuple<Args...>& argsTuple = *reinterpret_cast<std::tuple<Args...>*>(Frame->GetArgsTuple());
        std::tuple<Ret>& retTuple = *reinterpret_cast<std::tuple<Ret>*>(Frame->GetRetTuple());
        std::get<0>(retTuple) = std::apply((CFunctionPtr)Ptr, argsTuple);  // c++17
    }

    virtual typename std::unique_ptr<CFunction::CFrame> NewFrame() override {
        return std::make_unique<typename TCppFunction<Ret, Args...>::CFrame>();
    }


public:

    TCppFunction(const std::string& name, CFunctionPtr ptr)
        : CFunction(name, ptr)
    {

    }

private:

    //std::any InvokeImpl(std::any, std::vector<std::any>& args) const override
    //{
    //    return InvokeImpl(args, std::index_sequence_for<Args...>());
    //}

    //template <size_t... indices>
    //InvokeImp2l(std::vector<Any>& args, std::index_sequence<indices...> indexSequence) const
    //{
    //    InvokePtr(*std::get<indices>(ArgsBuffer));
    //}


    //template <size_t... indices>
    //std::any InvokeImpl(const std::vector<std::any>& args, size_t argIndex, const std::tuple& tuple, Arg&& arg) const
    //{
    //    std::tuple;
    //    if constexpr (std::is_reference<Arg>())
    //    {
    //        Arg* argPtrType = &arg;
    //        std::tuple_cat(tuple, argPtrType);
    //    }
    //    else
    //    {
    //        std::tuple_cat(tuple, std::forward<Arg>(arg));
    //    }
    //}

    //template<typename Arg>
    //std::any InvokeImpl(const std::vector<std::any>& args, size_t argIndex, const std::tuple& tuple, Arg&& lastParam, Args&&... params) const
    //{
    //    if constexpr (std::is_reference<Arg>())
    //    {
    //        Arg* argPtr = std::any_cast<Arg>(&args[argIndex]);
    //        return InvokeImpl(std::tuple_cat(tuple, argPtr), argIndex, args);
    //    }
    //    else
    //    {
    //        return InvokeImpl(std::tuple_cat(tuple, std::forward<Arg>(std::any_cast<Arg>(args[argIndex]))), argIndex, args);
    //    }
    //}

    //template <size_t... indices>
    //std::any InvokeImpl(std::vector<std::any>& args, std::index_sequence<indices...> indexSequence) const
    //{
    //    std::tuple argsTuple = std::make_tuple(args[indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);
    //    std::vector<std::any> convertedArgs{ (std::get<indices>(argsTuple) ? AnyRef(*std::get<indices>(argsTuple)) : args[indices].TryConvert<std::remove_cv_t<std::remove_reference_t<Args>>>())... };
    //    argsTuple = std::make_tuple(convertedArgs[indices].TryCast<std::remove_cv_t<std::remove_reference_t<Args>>>()...);

    //    if ((std::get<indices>(argsTuple) && ...))  // all arguments are valid
    //        if constexpr (std::is_reference_v<Ret>)
    //            return AnyRef(mFreeFunPtr(*std::get<indices>(argsTuple)...));
    //        else
    //            return mFreeFunPtr(*std::get<indices>(argsTuple)...);
    //    else
    //        return Any();
    //}
};

//
//template <typename Ret, typename... Args>
//Ret TInvoke(CFunction* Function, Args&&... args)
//{
//    CFrame Frame = Function->NewFrame();
//    std::tuple<Args...>& argsTuple = *(Frame->ArgsTuplePtr);
//    argsTuple = std::make_tuple(std::forward<Args>(args)...);
//    Function->Invoke(Frame);
//    std::tuple<Ret>& retTuple = *(Frame->RetTuplePtr);
//    return std::get<0>(retTuple)
//}
//
//
//class CConstructor : public CFunction
//{
//    CConstructor(const std::string& name)
//        : CFunction(name)
//    {}
//
//};
#pragma once
#include <vector>
#include <memory>
#include "Record.h"
#include "Property.h"


class REFL_API CFunction : public CRecord
{
//public:
//    CFunction(const std::string& name, void* ptr)
//        : CMetadata(name)
//        , Ptr(ptr)
//    {}
//
//    struct CFrame
//    {
//        CFrame(){}
//        virtual ~CFrame() = default;
//        CFrame(const CFrame&) = delete;
//        CFrame(CFrame&&) = delete;
//        CFrame& operator=(const CFrame&) = delete;
//        CFrame& operator=(CFrame&&) = delete;
//
//        virtual void* GetParameter(uint32_t index = UINT32_MAX) = 0;
//
//        virtual void SetParameterByReference(uint32_t index, void* Reference) = 0;
//        virtual void SetParameterByValue(uint32_t index, void* Value) = 0;
//    };
//
//    virtual void Invoke(CFrame* Frame) = 0;
//    virtual std::unique_ptr<CFrame> NewFrame() = 0;
//
//
//
//    //template<typename T>
//    //T& GetParameter(CFrame* frame, CParameter* parameter)
//    //{
//    //    return *(T*)(frame->GetArgumentPtr(parameter->Index));
//    //}
//
//    //template<typename T>
//    //T& GetParameter(CFrame* frame, size_t index)
//    //{
//    //    return *(T*)(frame->GetArgumentPtr(index));
//    //}
//
//    //template<typename T>
//    //void SetParameter(CFrame* frame, CParameter* parameter, T&& value)
//    //{
//    //    GetParameter<T>(frame, parameter) = std::forward<T>(value);
//    //}
//
//    //template<typename T>
//    //void SetParameter(CFrame* frame, size_t index, T&& value)
//    //{
//    //    GetParameter<T>(frame, Params[index]) = std::forward<T>(value);
//    //}
//
//protected:
//    void* Ptr;
//    CParameter* ReturnValue;
//    std::vector<CParameter*> Params;
};
//
//template<typename T>
//struct TTemplateArgument
//{};
//
//template<typename T>
//struct TReferenceHelper
//{
//    static T Value;
//};
//
//template<typename T>
//T TReferenceHelper<T>::Value;
//
//template <typename Ret, typename... Args>
//class TCppFunction : public CFunction
//{
//private:
//    using CFunctionPtr = Ret(*)(Args...);
//public:
//    struct CFrame : public CFunction::CFrame
//    {
//        friend class TCppFunction<Ret, Args...>;
//        using RetStorage = std::tuple<std::conditional_t<std::is_reference_v<Ret>, std::reference_wrapper<std::remove_reference_t<Ret>>, Ret>>;;
//        using ArgStorage = std::tuple<std::conditional_t<std::is_reference_v<Args>, std::reference_wrapper<std::remove_reference_t<Args>>, Args> ...>;
//    private:
//        
//    public:
//        CFrame()
//            : CFunction::CFrame()
//            , ReturnValue(std::remove_const_t<std::remove_reference_t<Ret>>())
//            , Argument(TReferenceHelper<std::remove_cv_t<std::remove_reference_t<Args>>>::Value ...)
//        {
//            ResolveParameter<0>(TTemplateArgument<Args>()...);
//        }
//
//        ~CFrame()
//        {
//        }
//        
//        template<size_t N, typename T>
//        void ResolveParameter(TTemplateArgument<T> argument)
//        {
//            Parameter[N] = std::addressof(std::get<N>(Argument));
//            Parameter[sizeof...(Args)] = std::addressof(std::get<0>(ReturnValue));
//        }
//        
//        template<size_t N, typename T, typename ...Arguments>
//        void ResolveParameter(TTemplateArgument<T> argument, TTemplateArgument<Arguments>... arguments)
//        {
//            if constexpr (std::is_reference_v<T>)  Parameter[N] = std::addressof(std::get<N>(Argument).get());
//            else Parameter[N] = std::addressof(std::get<N>(Argument));
//            ResolveParameter<N + 1>(arguments...);
//        }
//
//        virtual void* GetParameter(uint32_t index) override
//        { 
//            if (index > sizeof...(Args))
//                return Parameter[sizeof...(Args)];
//            return Parameter[index];
//        }
//
//        virtual void SetParameterByReference(uint32_t index, void* Reference)override
//        {
//
//        }
//        
//        virtual void SetParameterByValue(uint32_t index, void* Value)
//        {
//        
//        }
//
//        class CParameterAccessor
//        {
//            virtual void Set(void*) = 0;
//
//        protected:
//            void* Ptr;
//        };        
//        template<typename T>
//        class TParameterAccessor
//        {
//            virtual void Set(void* ValuePtr) override
//            {
//                if constexpr (std::is_reference_v<T>)
//                {
//                    std::reference_wrapper<std::remove_reference_t<T>>& RefWrapper = (std::reference_wrapper<std::remove_reference_t<T>>*)Ptr;
//                     RefWrapper = *(std::remove_reference_t<T>*)ValuePtr;
//                }
//                else
//                {
//                    *(T*)Ptr = *(T*)ValuePtr;
//                }
//            }
//        };
//
//    private:
//        void* Parameter[sizeof...(Args) + 1];
//        RetStorage ReturnValue;
//        ArgStorage Argument;
//    };
//
//    virtual void Invoke(CFunction::CFrame* Frame) override
//    {
//        TCppFunction<Ret, Args...>::CFrame* LocalFrame = reinterpret_cast<TCppFunction<Ret, Args...>::CFrame*>(Frame);
//        std::get<0>(LocalFrame->ReturnValue) = std::apply((CFunctionPtr)Ptr, LocalFrame->Argument);  // c++17
//    }
//
//    virtual typename std::unique_ptr<CFunction::CFrame> NewFrame() override {
//        return std::make_unique<typename TCppFunction<Ret, Args...>::CFrame>();
//    }
//
//private:
//    template<typename T>
//    void ResolveParameter(TTemplateArgument<T> argument)
//    {
//        static CParameter Parameter(typeid(T).name(), nullptr, Params.size());
//        Params.push_back(&Parameter);
//        static CParameter ReturnParameter(typeid(Ret).name(), nullptr, Params.size());
//        Params.push_back(&ReturnParameter);
//        ReturnValue = &ReturnParameter;
//    }
//
//    template<typename T, typename ...Arguments>
//    void ResolveParameter(TTemplateArgument<T> argument, TTemplateArgument<Arguments>... arguments)
//    {
//        static CParameter Parameter(typeid(T).name(), nullptr, Params.size());
//        Params.push_back(&Parameter);
//        ResolveParameter(arguments...);
//    }
//public:
//
//    TCppFunction(const std::string& name, CFunctionPtr ptr)
//        : CFunction(name, ptr)
//    {
//        ResolveParameter(TTemplateArgument<Args>()...);
//    }
//
//private:
//
//
//};
//
////template <typename Ret, typename... Args>
////class TCppFunction : public CFunction
////{
////private:
////    using CFunctionPtr = Ret(*)(Args...);
////public:
////    struct CFrame : public CFunction::CFrame
////    {
////        friend class TCppFunction<Ret, Args...>;
////    private:
////        template<typename T>
////        struct TArgument
////        {};
////
////
////        template<typename T>
////        void NewArgsBuffers(size_t index, TArgument<T> argument)
////        {
////            if      constexpr (std::is_rvalue_reference_v<T>) ArgsPtr[index] = new std::remove_reference_t<T>();
////            else if constexpr (std::is_lvalue_reference_v<T>) ArgsPtr[index] = new std::reference_wrapper<std::remove_reference_t<T>>();
////            else if constexpr (std::is_pointer_v<T>) (void)0;
////            else                                              ArgsPtr[index] = new T();
////        }
////
////        template<typename T, typename ...Arguments>
////        void NewArgsBuffers(size_t index, TArgument<T> argument, TArgument<Arguments>... arguments)
////        {
////            if      constexpr (std::is_rvalue_reference_v<T>) ArgsPtr[index] = new std::remove_reference_t<T>();
////            else if constexpr (std::is_lvalue_reference_v<T>) ArgsPtr[index] = new std::reference_wrapper<std::remove_reference_t<T>>();
////            else if constexpr (std::is_pointer_v<T>) (void)0;
////            else                                              ArgsPtr[index] = new T();
////            NewArgsBuffers(index + 1, arguments...);
////        }
////
////        template<typename T>
////        void FreeArgsBuffers(size_t index, TArgument<T> argument)
////        {
////            if      constexpr (std::is_rvalue_reference_v<T>) delete (std::remove_reference_t<T>*)(ArgsPtr[index]);
////            else if constexpr (std::is_lvalue_reference_v<T>) delete (std::reference_wrapper<std::remove_reference_t<T>>*)ArgsPtr[index];
////            else if constexpr (std::is_pointer_v<T>) (void)0;
////            else                                              delete (T*)(ArgsPtr[index]);
////        }
////
////        template<typename T, typename ...Arguments>
////        void FreeArgsBuffers(size_t index, TArgument<T> argument, TArgument<Arguments>... arguments)
////        {
////            if      constexpr (std::is_rvalue_reference_v<T>) delete (std::remove_reference_t<T>*)(ArgsPtr[index]);
////            else if constexpr (std::is_lvalue_reference_v<T>) delete (std::reference_wrapper<std::remove_reference_t<T>>*)ArgsPtr[index];
////            else if constexpr (std::is_pointer_v<T>) (void)0;
////            else                                              delete (T*)(ArgsPtr[index]);
////            FreeArgsBuffers(index + 1, arguments...);
////        }
////
////        template<typename T>
////        std::tuple<T> MakeArgsTuple(size_t index, TArgument<T> argument)
////        {
////            if      constexpr (std::is_rvalue_reference_v<T>) return std::tuple<T>(*(std::remove_reference_t<T>*)ArgsPtr[index]);
////            else if constexpr (std::is_lvalue_reference_v<T>) return std::tuple<T>(((std::reference_wrapper<std::remove_reference_t<T>>*)ArgsPtr[index])->get());
////            else if constexpr (std::is_pointer_v<T>) return std::tuple<T>((T)ArgsPtr[index]);
////            else                                              return std::tuple<T>(*(T*)ArgsPtr[index]);
////        }
////
////        template<typename T, typename ...Arguments>
////        std::tuple<T, Arguments...> MakeArgsTuple(size_t index, TArgument<T> argument, TArgument<Arguments>... arguments)
////        {
////            if      constexpr (std::is_rvalue_reference_v<T>) return std::tuple_cat(std::tuple<T>(*(std::remove_reference_t<T>*)ArgsPtr[index]), MakeArgsTuple(index + 1, arguments...));
////            else if constexpr (std::is_lvalue_reference_v<T>) return std::tuple_cat(std::tuple<T>(((std::reference_wrapper<std::remove_reference_t<T>>*)ArgsPtr[index])->get()), MakeArgsTuple(index + 1, arguments...));
////            else if constexpr (std::is_pointer_v<T>) return std::tuple_cat(std::tuple<T>((T)ArgsPtr[index]), MakeArgsTuple(index + 1, arguments...));
////            else                                              return std::tuple_cat(std::tuple<T>(*(T*)ArgsPtr[index]), MakeArgsTuple(index + 1, arguments...));
////        }
////
////    public:
////        CFrame()
////            : CFunction::CFrame()
////        {
////            NewArgsBuffers(0, TArgument<Args>()...);
////            if constexpr (!std::is_pointer_v<Ret> && !std::is_reference_v<Ret>) RetPtr = new Ret();
////        }
////
////        ~CFrame()
////        {
////            FreeArgsBuffers(0, TArgument<Args>()...);
////            if constexpr (!std::is_pointer_v<Ret> && !std::is_reference_v<Ret>) delete (Ret*)RetPtr;
////        }
////
////        virtual void* GetReturnValuePtr() override { return RetPtr; }
////        virtual void* GetArgumentPtr(uint32_t index) override { return ArgsPtr[index]; }
////
////    private:
////        void* RetPtr;
////        void* ArgsPtr[sizeof...(Args)];
////
////    };
////
////    virtual void Invoke(CFunction::CFrame* Frame) override
////    {
////        std::apply(
////            (CFunctionPtr)Ptr,
////            reinterpret_cast<typename TCppFunction<Ret, Args...>::CFrame*>(Frame)->MakeArgsTuple(
////                0,
////                typename TCppFunction<Ret, Args...>::CFrame::template TArgument<Args>()...));  // c++17
////    }
////
////    virtual typename std::unique_ptr<CFunction::CFrame> NewFrame() override {
////        return std::make_unique<typename TCppFunction<Ret, Args...>::CFrame>();
////    }
////
////
////public:
////
////    TCppFunction(const std::string& name, CFunctionPtr ptr)
////        : CFunction(name, ptr)
////    {
////
////    }
////
////private:
////};

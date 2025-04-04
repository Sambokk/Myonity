#pragma once
#include "TypeInfoBase.h"

// 클래스 타입 정보를 담고 있는 TypeInfo 객체를 생성
#define GENERATE_CLASS_TYPE_INFO ( TypeName )   \
private : \
	friend SuperClassTypeDeduction; \
	friend TypeInfoInitializer; \
\
public: \
	using Super = typename SuperClassTypeDeduction<Typename>::Type; \
	using ThisType = TypeName; \
\
	static TypeInfo& StaticTypeInfo() \
	{	\
		static TypeInfo typeInfo{ TypeInfoInitializer<ThisType>( #TypeName) }; \
	}	\
\
	virtual const TypeInfo& GetTypeInfo() const \
	{	\
		return m_typeInfo; \
	}	\
\
private: \
	inline static TypeInfo& m_typeInfo = StaticTypeInfo(); \
	\
private: \



// 변수 정보를 생성
#define PROPERTY( Name ) \
	inline static struct RegistPropertyExecutor_##Name \




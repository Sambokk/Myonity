#pragma once

class TypeInfo;
struct TypeInfoInitializer;


/// <summary>
/// 실제 타입 정보를 담는 클래스.
/// 다중 상속은 지원하지 않는다.
/// </summary>
class TypeInfo
{
public:
	template <typename T>
	explicit TypeInfo(const TypeInfoInitializer<T>& initializer) :
		m_typeHash(typeid( T ).hash_code())
	{

	}

	/// TypeInfo는 Static 변수이기 때문에 대부분 주소검사로 확인이 가능하지만
/// 다른 dll의 같은 타입의 경우에는 처리가 안되기 때문에
/// typeid로 얻은 해시 값도 비교한다.

// TypeInfo가 동일한 타입인지 검사함. 
	bool IsA(const TypeInfo& other) const
	{
		if ( this == &other )
		{
			return true;
		}

		return m_typeHash == other.m_typeHash;
	}

	bool IsChildOf(const TypeInfo& other) const
	{
		if ( IsA(other) )
		{
			return true;
		}

		for ( const TypeInfo* super = m_super; super != nullptr; super = super->GeSuper() )
		{
			if ( super->IsA(other) )
			{
				return true;
			}
		}

		return false;
	}

	template <typename T>
	bool IsA() const
	{
		return IsA(GetStaticTypeInfo<T>());
	}

	template <typename T>
	bool IsChildOf() const
	{
		return IsChildOf(GetStaticTypeInfo<T>());
	}

	// 일부 코드 생략?
private:
	friend Method;
	friend Property;
	using MethodMap = std::map<std::string_view, const Method*>;
	using PropertyMap = std::map<std::string_view, const Property*>;

	// 일부 코드 생략?

	size_t m_typeHash;
	const char* m_name = nullptr;
	std::string m_fullName;
	const TypeInfo* m_super = nullptr;

	std::vector<const Method*> m_methods;
	MethodMap m_methodMap;

	std::vector<const Property*> m_properties;
	PropertyMap m_propertyMap;
};


/// std::void_t를 이용한 SFINAE를 통해 해당 타입에 ThisType 별칭이 있다면,
/// 그 타입을 부모 클래스의 타입으로 하고 없다면 void를 사용하도록 한다.
///
/// ThisType을 부모 타입으로 할 수 있는 이유는
/// 자신의 클래스 내에서 ThisType을 선언하기 전까지는
/// 부모로 부터 상속받은 ThisType이 사용되기 때문이다.

/// 즉 부모의 ThisType이 사용되기에 자식 입장에서는 Super가 된다.


// 부모 클래스의 타입을 가져오기 위해 사용되는 구조체
template <typename T, typename U = void>
struct SuperclassTypeDeduction
{
	using Type = void;
};

template <typename T>
struct SuperClassTypeDeduction<T, std::void_t<typename T::ThisType>>
{
	using Type = T::ThisType;
};


//----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
concept HasSuper = requires
{
	typename T::Super;
} && !std::same_as<typename T::Super, void>;

//----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
struct TypeInfoInitializer
{
	TypeInfoInitializer (const char* name )
		: m_name(name)
	{
		// Super 타입 별칭이 없거나 void이면 부모 타입의 TypeInfo는 nullptr이 됩니다. 
		if constexpr ( HasSuper<T> )
		{
			m_super = &( typename T::Super::StaticTypeInfo() );
		}
	}

	const char* m_name = nullptr;
	const TypeInfo* m_super = nullptr;
};

//----------------------------------------------------------------------------------------------------------------------------------------------

template <typename To, typename From>
To* Cast(From* src)
{
	return src && src->GetTypeInfo().IsChildOf< To >() ? reinterpret_cast< To* >( src ) : nullptr;
}



//----------------------------------------------------------------------------------------------------------------------------------------------

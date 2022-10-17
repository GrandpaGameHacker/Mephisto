#pragma once
#include <Types.h>
#include <typeinfo>
#include <string>
#include <memory>
#include <unordered_map>
#include <spdlog/spdlog.h>
namespace
{
	inline static uint64 NextUID = 0;
}

template <class T>
class GameObject
{
public:
	friend class ObjectFactory;

	GameObject<T>();

	std::size_t GetTypeHash() const;
	std::string& GetTypeName() const;

	uint64 GetUniqueID() const;
	std::string GetName() const;
	std::shared_ptr<T> Get();
	std::weak_ptr<T> GetWeak();

protected:
	uint64 UID = 0;
	std::string ObjectName;
	std::shared_ptr<T> Object;

	inline static std::string ObjectType = "UNKNOWN_TYPE";
	inline static std::size_t ObjectHash = 0;
};

template<class T>
inline GameObject<T>::GameObject()
{
	ObjectType = std::string(typeid(T).name());
	ObjectHash = typeid(T).hash_code();
}


template<class T>
inline std::size_t GameObject<T>::GetTypeHash() const
{
	return ObjectHash;
}

template<class T>
inline uint64 GameObject<T>::GetUniqueID() const
{
	return UID;
}

template<class T>
inline std::string& GameObject<T>::GetTypeName() const
{
	return ObjectType;
}

template<class T>
inline std::shared_ptr<T> GameObject<T>::Get()
{
	if (Object)
	{
		return Object;
	}
	else
	{
		spdlog::error("Object type {} with id {} doesn't exist anymore", GetTypeName(), GetUniqueID());
		return nullptr;
	}
}
template<class T>
std::weak_ptr<T> GameObject<T>::GetWeak()
{
	if (Object) {
		return std::weak_ptr<T>(Object);
	}
}

template<class T>
inline std::string GameObject<T>::GetName() const
{
	return ObjectName;
}


class ObjectFactory
{
public:
	template <class T>
	static GameObject<T> Create()
	{
		GameObject<T> Obj;
		Obj.UID = NextUID++;
		Obj.ObjectName = std::vformat("{}_{}", std::make_format_args(Obj.ObjectType, Obj.UID));
		Obj.Object = std::make_shared<T>();
		spdlog::info("Created object:\n Type:\"{}\" | Hash: {} | UID: {} | Name: {}", Obj.GetTypeName(), Obj.GetTypeHash(), Obj.GetUniqueID(), Obj.GetName());
		return Obj;
	}

	template <class T, typename... Args>
	static GameObject<T> Create(Args && ... args)
	{
		GameObject<T> Obj;
		Obj.UID = NextUID++;
		Obj.ObjectName = std::vformat("{}_{}", std::make_format_args(Obj.ObjectType, Obj.UID));
		Obj.Object = std::make_shared<T>(args...);
		spdlog::info("Created object:\n Type:\"{}\" | Hash: {} | UID: {} | Name: {}", Obj.GetTypeName(), Obj.GetTypeHash(), Obj.GetUniqueID(), Obj.GetName());
		return Obj;
	}

	template <class T>
	static GameObject<T> CreateNamed(std::string name)
	{
		GameObject<T> Obj;
		Obj.UID = NextUID++;
		Obj.ObjectName = name;
		Obj.Object = std::make_shared<T>();
		spdlog::info("Created object:\n type \"{}\" with typehash {}, unique id {} and name {}", Obj.GetTypeName(), Obj.GetTypeHash(), Obj.GetUniqueID(), Obj.GetName());
		return Obj;
	}

	template <class T, typename... Args>
	static GameObject<T> CreateNamed(std::string name, Args && ... args)
	{
		GameObject<T> Obj;
		Obj.UID = NextUID++;
		Obj.ObjectName = std::vformat("{}_{}", std::make_format_args(Obj.ObjectType, Obj.UID));
		Obj.Object = std::make_shared<T>(args...);
		spdlog::info("Created object:\n Type:\"{}\" | Hash: {} | UID: {} | Name: {}", Obj.GetTypeName(), Obj.GetTypeHash(), Obj.GetUniqueID(), Obj.GetName());
		return Obj;
	}
};

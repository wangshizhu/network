#ifndef NETWORK_DEFINE_H_
#define NETWORK_DEFINE_H_

#include <msgpack.hpp>

namespace network
{
	typedef uint16	MessageID;
	typedef uint32	MessageLength;

#pragma pack(push,1)
	struct MsgHeader
	{
		MsgHeader(MessageID id, MessageLength l) : msg_id(id), msg_len(l) 
		{
		}
		MessageID msg_id;
		MessageLength msg_len;
	};

	struct MsgBase
	{
		MsgBase(MessageID id, MessageLength l) : msg_id(id), msg_len(l)
		{
		}

		MessageLength MsgLen()const { return msg_len; }
		MessageID MsgId()const { return msg_id; }

		MessageID msg_id;
		MessageLength msg_len;

	};
#pragma pack(pop)

	struct MsgBaseEx
	{
		MsgBaseEx(MessageID id)
		{
			msg_id = id;
		}
		virtual ~MsgBaseEx() {}
		virtual void Write(msgpack::packer<msgpack::sbuffer>& pack)const {}
		virtual void Read(msgpack::object& obj) {};

		MessageID MsgId()const { return msg_id; }

		MessageID msg_id;
	};
	
	#define INLINE inline
	#define MESSAGE_ID_SIZE	sizeof(network::MessageID)
	#define MESSAGE_LENGTH_SIZE	sizeof(network::MessageLength)
	#define MESSAGE_HEAD_SIZE	sizeof(MsgHeader)
	#define INVALID -1
	#define MESSAGE_MEMORY_DEF_SIZE 256
	#define PACKET_MAX_SIZE_TCP	1460
	#define PACKET_MAX_SIZE_UDP	1472

#define SAFE_RELEASE(i)										\
	if (i)													\
		{													\
			delete i;										\
			i = NULL;										\
		}

#define SAFE_RELEASE_ARRAY(i)								\
	if (i)													\
		{													\
			delete[] i;										\
			i = NULL;										\
		}

	template<class T>
	inline void ReadMapValue(msgpack::object& obj, std::string key, T& value)
	{
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					//if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_STR))
					pv.convert(value);
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	template<>
	inline void ReadMapValue(msgpack::object& obj, std::string key, std::string& value)
	{
		value = "";
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_STR))
						pv.convert(value);
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	template<>
	inline void ReadMapValue(msgpack::object& obj, std::string key, int& value)
	{
		value = 0;
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
						pv.convert(value);
					//else if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_NEGATIVE_INTEGER))
					//	pv.convert(value);
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	template<>
	inline void ReadMapValue(msgpack::object& obj, std::string key, short& value)
	{
		value = 0;
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
						pv.convert(value);
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	template<>
	inline void ReadMapValue(msgpack::object& obj, std::string key, bool& value)
	{
		value = false;
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_BOOLEAN))
						pv.convert(value);
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	template<>
	inline void ReadMapValue(msgpack::object& obj, std::string key, char& value)
	{
		value = 0;
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
						pv.convert(value);
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	//从map里面读取一个对象，目前只支持读取map array
	inline void ReadMapValue(msgpack::object& obj, std::string key, msgpack::object& value)
	{
		value = msgpack::object();
		msgpack::object_kv*  pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;
		if (static_cast<int>(obj.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
		{
			pkv = obj.via.map.ptr;
			pkv_end = obj.via.map.ptr + obj.via.map.size;
			do
			{
				pk = pkv->key;
				pv = pkv->val;
				if (pk.as<std::string>() == key)
				{
					if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY)
						|| static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_MAP))
						value = pv;
					break;
				}
				++pkv;
			} while (pkv < pkv_end);
		}
	}

	template<class T>
	inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, T& value)
	{
		msgpack::object*      pav;
		msgpack::object_kv*	 pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;

		if (static_cast<int>(array.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			if (index < array.via.array.size)
			{
				pav = array.via.array.ptr + index;
				if (static_cast<int>(pav->type) == static_cast<int>(MSGPACK_OBJECT_MAP))
				{
					pkv = pav->via.map.ptr;
					pkv_end = pav->via.map.ptr + pav->via.map.size;
					do
					{
						pk = pkv->key;
						pv = pkv->val;
						if (pk.as<std::string>() == key)
						{
							//if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
							pv.convert(value);
							break;
						}
						++pkv;
					} while (pkv < pkv_end);
				}
			}
		}
	}

	template<>
	inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, int& value)
	{
		value = 0;
		msgpack::object*      pav;
		msgpack::object_kv*	 pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;

		if (static_cast<int>(array.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			if (index < array.via.array.size)
			{
				pav = array.via.array.ptr + index;
				if (static_cast<int>(pav->type) == static_cast<int>(MSGPACK_OBJECT_MAP))
				{
					pkv = pav->via.map.ptr;
					pkv_end = pav->via.map.ptr + pav->via.map.size;
					do
					{
						pk = pkv->key;
						pv = pkv->val;
						if (pk.as<std::string>() == key)
						{
							if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
								pv.convert(value);
							break;
						}
						++pkv;
					} while (pkv < pkv_end);
				}
			}
		}
	}

	template<>
	inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, short& value)
	{
		value = 0;
		msgpack::object*     pav;
		msgpack::object_kv*	 pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;

		if (static_cast<int>(array.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			if (index < array.via.array.size)
			{
				pav = array.via.array.ptr + index;
				if (static_cast<int>(pav->type) == static_cast<int>(MSGPACK_OBJECT_MAP))
				{
					pkv = pav->via.map.ptr;
					pkv_end = pav->via.map.ptr + array.via.map.size;
					do
					{
						pk = pkv->key;
						pv = pkv->val;
						if (pk.as<std::string>() == key)
						{
							if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
							{
								pv.convert(value);
							}
							break;
						}
						++pkv;
					} while (pkv < pkv_end);
				}
			}
		}
	}

	template<>
	inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, std::string& value)
	{
		value = "";
		msgpack::object*     pav;
		msgpack::object_kv*	 pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;

		if (static_cast<int>(array.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			if (index < array.via.array.size)
			{
				pav = array.via.array.ptr + index;
				if (static_cast<int>(pav->type) == static_cast<int>(MSGPACK_OBJECT_MAP))
				{
					pkv = pav->via.map.ptr;
					pkv_end = pav->via.map.ptr + array.via.map.size;
					do
					{
						pk = pkv->key;
						pv = pkv->val;
						if (pk.as<std::string>() == key)
						{
							if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_STR))
								pv.convert(value);
							break;
						}
						++pkv;
					} while (pkv < pkv_end);
				}
			}
		}
	}

	template<>
	inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, char& value)
	{
		value = 0;
		msgpack::object*     pav;
		msgpack::object_kv*	 pkv;
		msgpack::object_kv*  pkv_end;
		msgpack::object      pk, pv;

		if (static_cast<int>(array.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			if (index < array.via.array.size)
			{
				pav = array.via.array.ptr + index;
				if (static_cast<int>(pav->type) == static_cast<int>(MSGPACK_OBJECT_MAP))
				{
					pkv = pav->via.map.ptr;
					pkv_end = pav->via.map.ptr + array.via.map.size;
					do
					{
						pk = pkv->key;
						pv = pkv->val;
						if (pk.as<std::string>() == key)
						{
							if (static_cast<int>(pv.type) == static_cast<int>(MSGPACK_OBJECT_POSITIVE_INTEGER))
								pv.convert(value);
							break;
						}
						++pkv;
					} while (pkv < pkv_end);
				}
			}
		}
	}

	template<typename T>
	inline void ReadAllArrayValue(msgpack::object& array, std::string key, std::vector<T>& value)
	{
		msgpack::object array1;
		ReadMapValue(array, key, array1);
		if (static_cast<int>(array1.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			for (std::size_t i = 0; i < array1.via.array.size; ++i)
			{
				T v;
				msgpack::object& pv = *(array1.via.array.ptr + i);
				pv.convert(v);
				value.emplace_back(std::move(v));
			}
		}
	}

	template<typename T>
	inline void ReadAllArrayValue2RawArray(msgpack::object& array, const std::string& key, T value[], std::size_t length)
	{
		msgpack::object array1;
		ReadMapValue(array, key, array1);
		if (static_cast<int>(array1.type) == static_cast<int>(MSGPACK_OBJECT_ARRAY))
		{
			if (length < array1.via.array.size)
			{
				return;
			}
			for (std::size_t i = 0; i < array1.via.array.size; ++i)
			{
				T v;
				msgpack::object& pv = *(array1.via.array.ptr + i);
				pv.convert(v);
				value[i] = std::move(v);
			}
		}
	}

	enum class EnumPoller
	{
		SELECT_POLLER = 0,
		POLL_POLLER,
		EPOLL_POLLER,
	};

	enum class EnumIpProto
	{
		ENUM_TCP = 0,
		ENUM_UDP
	};

	enum EnumReason
	{
		ENUM_SUCCESS = 0,
		ENUM_INVALID_VARIABLE,
		ENUM_SEND_FAILED,
		ENUM_SEND_CONTINUE,
		ENUM_NO_MSG_WAITING_SEND,
		ENUM_NO_MSG_WAITING_PROCESS,
		ENUM_WAITING_RECV,
	};

	enum EnumAppProto
	{
		ENUM_BUFF = 0,
		ENUM_MSGPACK,
		ENUM_PROTOBUF,
	};

}

#define READ_MAP_DATA(obj,key,data) network::ReadMapValue(obj,key,data);

#define READ_ARRAY_DATA(obj,i,key,data) network::ReadArrayValue(obj,i,key,data);

#define READ_ALL_ARRAY_DATA(obj,key,data) network::ReadAllArrayValue(obj,key,data);

#define WRITE_KEY_VALUE(pack,key,value) pack.pack(key);pack.pack(value);

#define WRITE_DATA(pack,data) pack.pack(data);

#define WRITE_MAP(pack,size) pack.pack_map(size);

#define WRITE_ARRAY(pack,size) pack.pack_array(size);

#define NAME_TO_STR(var)  (#var)

#endif
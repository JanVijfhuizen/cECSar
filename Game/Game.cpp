#include <CecsarRevamped.h>

template <typename ...Args>
class ColdSet final
{
public:
	std::tuple<Args*...> args{};
	
	constexpr ColdSet(const int32_t capacity)
	{
		InitMembers<sizeof...(Args) - 1>(capacity);
	}

	inline ~ColdSet()
	{
		DeleteMembers<sizeof...(Args) - 1>();
	}

	template <size_t S>
	constexpr auto Get()
	{
		return std::get<S>(args);
	}
	
	constexpr void ClearAt(const int32_t index)
	{
		ClearMembers<sizeof...(Args) - 1>(index);
	}

private:
	template <size_t S>
	constexpr void InitMembers(const int32_t capacity)
	{
		InitMember(std::get<S>(args), capacity);
		if constexpr (S > 0)
			InitMembers<S - 1>(capacity);
	}

	template <typename T>
	static constexpr void InitMember(T*& member, const int32_t capacity)
	{
		member = new T[capacity];
	}

	template <size_t S>
	constexpr void DeleteMembers()
	{
		DeleteMember(std::get<S>(args));
		if constexpr (S > 0)
			DeleteMembers<S - 1>();
	}

	template <typename T>
	static constexpr void DeleteMember(T*& member)
	{
		delete [] member;
	}

	template <size_t S>
	constexpr void ClearMembers(const int32_t index)
	{
		ClearMember(std::get<S>(args), index);
		if constexpr (S > 0)
			ClearMembers<S - 1>(index);
	}

	template <typename T>
	static constexpr void ClearMember(T*& member, const int32_t index)
	{
		member[index] = 0;
	}
};

using ColdTransform = ColdSet<bool, bool, float>;

enum ColdTransformMem
{
	parented, layer, z
};

struct SomeComponent
{
	
};

struct SomeComponentCold : public revamped::Cecsar::ColdSet<float, int, bool>
{
	
};

int main(int argc, char* argv[])
{
	//ColdTransform s{20};

	//auto* const b = s.Get<parented>();
	//b[10] = true;	

	//s.ClearAt(10);

	revamped::Cecsar shizaa{};
	auto& a = shizaa.GetSet<SomeComponent>();
	auto& b = shizaa.GetMapSet<SomeComponent>();
	auto& c = shizaa.GetColdSet<SomeComponent, SomeComponentCold>();
	
	return 0;
}

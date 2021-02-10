#include <CecsarRevamped.h>

struct Transform
{
	float x, y, z;
};

struct TransformSoA : public revamped::Cecsar::SoASet<float, float, float, revamped::Cecsar::Entity>
{
	 
};

enum TransformSoAMembers
{
	x, y, z, parent
};

int main(int argc, char* argv[])
{
	//ColdTransform s{20};

	//auto* const b = s.Get<parented>();
	//b[10] = true;	

	//s.ClearAt(10);

	revamped::Cecsar shizaa{};
	auto& a = shizaa.GetSet<Transform>();
	auto& b = shizaa.GetMapSet<Transform>();
	auto& c = shizaa.GetColdSet<TransformSoA>();

	const auto arr = c.Get<parent>();
	auto& i = arr[20] = revamped::Cecsar::Entity();

	shizaa.Destroy(20);
	
	
	return 0;
}

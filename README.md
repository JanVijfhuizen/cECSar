This is a lightweight Entity Component System (ECS) framework, meant to make the Data Oriented workflow more scalable for games.
It's a work in progress, but feel free to use it.

The framework is a very minimal ECS implementation, and is designed to be expanded upon.
Although it does provide some different types of sets to get started with, like the sparse set.

It's designed to minimize boilerplate code, and I've tried to keep it as straightforward as possible.
To support this design, lazy initialization and singletons are at the heart of this framework.

Setting up Cecsar is pretty straightforward:

```
// Define a component.
struct Transform
{
	float x, y, z;
}

int main()
{
	// Define the framework.
	jecs::Cecsar cecsar;
	
	// Spawn an entity.
	Entity entity = cecsar.Spawn();
	
	// Get the transform set, and add a transform component to the spawned entity.
	auto& transforms = SparseSet<Transform>::Get();
	Transform& transform = transforms.Insert(entity.index);
	transform.y = 5;
	
	// Iterate over all the transforms.
	for(auto [transform, index] : transforms)
	{
		// Do something.
	}

	return 0;
}
```

I've included a few example scripts in the project.
Please look at those scripts, as they detail how the framework can be used in a more advanced way.
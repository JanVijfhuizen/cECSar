#pragma once

namespace cecsar
{
	class Cecsar;

	/*
	OPTIONAL.

	Inherit this to be able to easily get it from cecsar with
	cecsar.GetModule<MyModule>.

	EXAMPLE:
	auto& renderModule = cecsar.GetModule<RenderModule>.
	renderModule.SetScreenSize(w, h);
	*/
	class IModule
	{
		friend Cecsar;

	public:
		virtual ~IModule();

	protected:
		virtual void Initialize(Cecsar& cecsar);
	};
}

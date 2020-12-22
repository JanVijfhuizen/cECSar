#pragma once
#include <IModule.h>
#include <Cecsar.h>

namespace game
{
	template <typename T>
	class BufferModule final : public cecsar::IModule
	{
	public:
		utils::SparseSet<T>* buffer = nullptr;

		void UpdateBuffer();

	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;

	private:
		utils::SparseSet<T>* _original = nullptr;
	};

	template <typename T>
	void BufferModule<T>::UpdateBuffer()
	{
		buffer->Clear();

		const auto iterator = _original->GetDenseIterator();
		for (auto it : iterator)
			buffer->Insert(it, _original->Get(it));
	}

	template <typename T>
	void BufferModule<T>::Initialize(cecsar::Cecsar& cecsar)
	{
		buffer = new utils::SparseSet<T>(cecsar.info.setCapacity);
		_original = &cecsar.GetSet<T>();
	}
}

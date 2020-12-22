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

		const int32_t* dense = _original->GetDenseRaw();
		const int32_t count = _original->GetCount();

		for (int32_t i = 0; i < count; ++i)
		{
			const int32_t index = dense[i];
			buffer->Insert(index, _original->operator[](i));
		}
	}

	template <typename T>
	void BufferModule<T>::Initialize(cecsar::Cecsar& cecsar)
	{
		buffer = new utils::SparseSet<T>(cecsar.info.setCapacity);
		_original = &cecsar.GetSet<T>();
	}
}

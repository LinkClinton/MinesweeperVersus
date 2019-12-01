#pragma once

#include "../RuntimeSharing.hpp"

#include <memory>

namespace Minesweeper {

	class Manager : public Noncopyable {
	public:
		explicit Manager(
			const std::shared_ptr<RuntimeSharing>& sharing) :
			mRuntimeSharing(sharing) {}

		~Manager() = default;

		virtual void update(float delta) {};
	protected:
		std::shared_ptr<RuntimeSharing> mRuntimeSharing;
	};
	
}
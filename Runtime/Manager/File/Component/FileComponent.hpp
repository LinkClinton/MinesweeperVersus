#pragma once

#include "../../Core/Interface/Noncopyable.hpp"

#include <string>

namespace Minesweeper {

	enum class FileComponentMask : unsigned {
		eLoad = 1,
		eSave = 2,
		eAll = eLoad | eSave
	};

	inline FileComponentMask operator&(const FileComponentMask& left, const FileComponentMask& right) {
		return FileComponentMask(static_cast<unsigned>(left) & static_cast<unsigned>(right));
	}

	inline FileComponentMask operator|(const FileComponentMask& left, const FileComponentMask& right) {
		return FileComponentMask(static_cast<unsigned>(left) | static_cast<unsigned>(right));
	}

	inline bool enumHas(const FileComponentMask& left, const FileComponentMask& right) {
		return (left & right) == right;
	}
	
	class FileComponent : public Noncopyable {
	public:
		explicit FileComponent(
			const std::string& fileName,
			const FileComponentMask& mask = FileComponentMask::eLoad) :
			mMask(mask), mFileName(fileName) {}

		~FileComponent() = default;

		void initialize() { if (enumHas(mMask, FileComponentMask::eLoad)) load(mFileName); }

		void finalize() { if (enumHas(mMask, FileComponentMask::eSave)) save(mFileName); }
		
		virtual void load(const std::string& fileName) = 0;

		virtual void save(const std::string& fileName) = 0;

		auto mask() const noexcept -> FileComponentMask { return mMask; }
	protected:
		FileComponentMask mMask = FileComponentMask::eLoad;
		
		std::string mFileName;
	};
	
}
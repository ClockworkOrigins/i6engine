#ifndef __I6ENGINE_PLUGINS_I6ARCHIVE_H__
#define __I6ENGINE_PLUGINS_I6ARCHIVE_H__

#include "OGRE/OgreArchive.h"

namespace i6engine {
namespace plugins {

	class i6ArchiveFactory;

	/**
	 * \class i6Archive is used to represent a container for all objects that need protection and shall be compressed
	 */
	class i6Archive : public Ogre::Archive {
		friend class i6ArchiveFactory;

	private:
		std::map<std::string, std::string> _files;

		i6Archive(const Ogre::String & name, const Ogre::String & archType) : Archive(name, archType), _files() {
			mReadOnly = true;
		}

		/**
		 * \brief Returns whether this archive is case sensitive in the way it matches files
		 * i6Archive isn't case sensitive, so always return false
		 */
		bool isCaseSensitive() const override {
			return false;
		}

		/**
		 * \brief Loads the archive.
		 * \remarks	This initializes all the internal data of the class.
		 * \warning Do not call this function directly, it is meant to be used only by the ArchiveManager class.
		 */
		void load() override;

		/**
		 * \brief Unloads the archive.
		 * \warning Do not call this function directly, it is meant to be used only by the ArchiveManager class.
		 */
		void unload() override;

		/**
		 * \brief Open a stream on a given file.
		 * \note There is no equivalent 'close' method; the returned stream controls the lifecycle of this file operation.
		 * \param filename The fully qualified name of the file
		 * \param readOnly Whether to open the file in read-only mode or not (note,	if the archive is read-only then this cannot be set to false)
		 * \return A shared pointer to a DataStream which can be used to read / write the file. If the file is not present, returns a null shared pointer.
		 */
		Ogre::DataStreamPtr open(const Ogre::String & filename, bool readOnly = true) const override;

		/**
		 * \brief List all file names in the archive.
		 * \note This method only returns filenames, you can also retrieve other information using listFileInfo.
		 * \param recursive Whether all paths of the archive are searched (if the archive has a concept of that)
		 * \param dirs Set to true if you want the directories to be listed instead of files
		 * \return A list of filenames matching the criteria, all are fully qualified
		 */
		Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false) override;

		/**
		 * \brief List all files in the archive with accompanying information.
		 * \param recursive Whether all paths of the archive are searched (if the archive has a concept of that)
		 * \param dirs Set to true if you want the directories to be listed instead of files
		 * \return A list of structures detailing quite a lot of information about all the files in the archive.
		 */
		Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false) override;

		/**
		 * \brief Find all file or directory names matching a given pattern in this archive.
		 * \note This method only returns filenames, you can also retrieve other information using findFileInfo.
		 * \param pattern The pattern to search for; wildcards (*) are allowed
		 * \param recursive Whether all paths of the archive are searched (if the archive has a concept of that)
		 * \param dirs Set to true if you want the directories to be listed instead of files
		 * \return A list of filenames matching the criteria, all are fully qualified
		 */
		Ogre::StringVectorPtr find(const Ogre::String & pattern, bool recursive = true, bool dirs = false) override;

		/**
		 * \brief Find out if the named file exists (note: fully qualified filename required)
		 */
		bool exists(const Ogre::String & filename) override;

		/**
		 * \brief Retrieve the modification time of a given file
		 * there are no file informations stored, so this will return some magic number
		 */
		time_t getModifiedTime(const Ogre::String & filename) override {
			return 0;
		}

		/**
		 * \brief Find all files or directories matching a given pattern in this archive and get some detailed information about them.
		 * \param pattern The pattern to search for; wildcards (*) are allowed
		 * \param recursive Whether all paths of the archive are searched (if the archive has a concept of that)
		 * \param dirs Set to true if you want the directories to be listed instead of files
		 * \return A list of file information structures for all files matching the criteria.
		 */
		Ogre::FileInfoListPtr findFileInfo(const Ogre::String & pattern, bool recursive = true, bool dirs = false) const override;
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_I6ARCHIVE_H__ */

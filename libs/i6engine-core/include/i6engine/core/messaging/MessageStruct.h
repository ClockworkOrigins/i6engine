/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_MESSAGESTRUCT_H__
#define __I6ENGINE_CORE_MESSAGESTRUCT_H__

#include "i6engine/core/messaging/IPKey.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#ifdef ISIXE_PROFILING
	#include "boost/date_time/posix_time/posix_time.hpp"
	#include "boost/date_time/posix_time/time_serialize.hpp"
#endif /* ISIXE_PROFILING */

#include "boost/serialization/export.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/vector.hpp"

#ifdef ISIXE_PROFILING
	#include "boost/thread/mutex.hpp"
#endif /* ISIXE_PROFILING */

/**
 * \TODO Docu
 */

namespace i6engine {
namespace core {

	typedef struct ISIXE_CORE_API MessageStruct {
		int64_t _id;
		IPKey _sender;
		int64_t _waitForId;

#ifdef ISIXE_PROFILING
		std::vector<std::pair<std::string, boost::posix_time::ptime>> _timestamps;
		mutable boost::mutex _lock;
#endif /* ISIXE_PROFILING */

		template<class Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & _id;
			ar & _sender;
			ar & _waitForId;
#ifdef ISIXE_PROFILING
			boost::mutex::scoped_lock sl(_lock);
			ar & _timestamps;
#endif /* ISIXE_PROFILING */
		}

#ifdef ISIXE_PROFILING
		/**
		 * \brief Default constructor for MessageStruct.
		 */
		MessageStruct() : _id(-1), _sender(), _waitForId(-1), _timestamps(), _lock() {
			assert(_timestamps.size() == 0);
			insertTimestamp("Create Default c'tor");
			assert(_timestamps.size() == 1);
		}

		/**
		 * \brief Copy constructor for MessageStruct
		 */
		MessageStruct(const MessageStruct & other): _id(other._id), _sender(other._sender), _waitForId(other._waitForId), _lock() {
			other._lock.lock();
			assert(_timestamps.size() == 0);
			_timestamps = other._timestamps;
			assert(_timestamps.size() == other._timestamps.size());
			insertTimestamp("Copy");
			assert(_timestamps.size() == other._timestamps.size() + 1);
			other._lock.unlock();
		}

		/**
		 * \brief Constructor for MessagStruct.
		 * \param id ID.
		 * \param send Sender.
		 * \param waitID waitForId.
		 */
		MessageStruct(const int64_t id, const IPKey & send = IPKey(), const int64_t waitID = -1) : _id(id), _sender(send), _waitForId(waitID), _timestamps(), _lock() {
			assert(_timestamps.size() == 0);
			insertTimestamp("Create");
			assert(_timestamps.size() == 1);
		}

		MessageStruct(int64_t id, int64_t waitID) : _id(id), _sender(), _waitForId(waitID), _timestamps(), _lock() {}
#else /* ISIXE_PROFILING */
		/**
		 * \brief Default constructor for MessageStruct.
		 */
		MessageStruct() : _id(-1), _sender(), _waitForId(-1) {}

		/**
		 * \brief Copy constructor for MessageStruct
		 */
		MessageStruct(const MessageStruct & other) : _id(other._id), _sender(other._sender), _waitForId(other._waitForId) {
		}

		/**
		 * \brief Constructor for MessageStruct.
		 * \param id ID.
		 * \param send Sender.
		 * \param waitID waitForId.
		 */
		MessageStruct(const int64_t id, const IPKey & send, const int64_t waitID) : _id(id), _sender(send), _waitForId(waitID) {
		}

		MessageStruct(int64_t id, int64_t waitID) : _id(id), _sender(), _waitForId(waitID) {
		}
#endif /* ISIXE_PROFILING */


		/**
		 * \brief Virtual destructor.
		 */
		virtual ~MessageStruct() {}

		/**
		 * \brief Copy method returning an exact copy of itself.
		 * \return Copy of itself.
		 */
		virtual MessageStruct * copy() { return new MessageStruct(*this); }

#ifdef ISIXE_PROFILING
		/**
		 * \brief inserts a new timestamp in the message
		 * \param[in] text short text describing the current location of the message
		 */
		void insertTimestamp(const std::string & text) {
			_lock.lock();
			size_t n = _timestamps.size();
			boost::posix_time::ptime time = boost::posix_time::microsec_clock::universal_time();
			_timestamps.push_back(std::make_pair(text, time));

			assert(_timestamps.size() == n + 1);
			_lock.unlock();
		}

		/**
		 * \brief gets the timesamps
		 * the timestamps are formatted to be human readable. Just print them
		 */
		std::string getTimestamps() {
			std::stringstream ret;
			ret << "\n\t\t" << _timestamps[0].first << "\n";
			for (size_t i = 1; i < _timestamps.size(); ++i) {
				boost::posix_time::time_duration diff = boost::posix_time::time_period(_timestamps[i - 1].second, _timestamps[i].second).length();
				if (diff.total_milliseconds() < 15) {
					ret << "\t[\x1b[32m" << boost::posix_time::to_simple_string(diff) << "\x1b[0m]\n";
				} else {
					ret << "\t[\x1b[31m" << boost::posix_time::to_simple_string(diff) << "\x1b[0m]\n";
				}
				ret << "\t\t" << _timestamps[i].first << "\n";
			}
			return ret.str();
		}
#endif /* ISIXE_PROFILING */

		/**
		 * \brief returns the id
		 */
		int64_t getID() const {
			return _id;
		}

		/**
		 * \brief returns the waitForId
		 */
		int64_t getWaitID() const {
			return _waitForId;
		}
	} MessageStruct;

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_MESSAGESTRUCT_H__ */

/**
 * @}
 */

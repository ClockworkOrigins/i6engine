#ifndef __I6ENGINE_TOOLS_COMMON_CONFIG_H__
#define __I6ENGINE_TOOLS_COMMON_CONFIG_H__

namespace i6e {
namespace tools {

	class Config {
	public:
		/**
		 * \brief always call after creation of QApplication in main and before creating MainWindow
		 */
		static void Init();
	};

} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_COMMON_CONFIG_H__ */

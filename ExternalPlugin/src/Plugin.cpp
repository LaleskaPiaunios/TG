#include "Plugin.h"

// TerraLib

// TerraLib
//#include "../../../common/Config.h"
//#include "../../../common/Translator.h"
//#include "../../af/ApplicationController.h"
//#include "../../af/events/LayerEvents.h"
//#include "../../af/Utils.h"
//#include "Plugin.h"
//#include "PluginDefaultDialog.h"

#include <terralib/qt/af/ApplicationController.h>

//#include "qt/MyDialog.h"
#include "qt/RasterFile.h"
#include "Utils.h"

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
//Log4cxx
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/logger.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/logstring.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/rollingfileappender.h>
#endif

// QT
#include <QMenu>
#include <QMenuBar>
#include <QString>

// STL
#include <string>

ep::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo),
    m_myAction(0)
{
}

ep::Plugin::~Plugin() 
{
}

void ep::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib VP Qt Plugin
  //TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_VP_TEXT_DOMAIN, TE_QT_PLUGIN_VP_TEXT_DOMAIN_DIR, "UTF-8");

  //TE_LOG_TRACE(TE_TR("TerraLib Qt VP Plugin startup!"));

  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Processing");

  m_myAction = new QAction(pluginMenu);
  m_myAction->setText("Remove Clouds");

  pluginMenu->addAction(m_myAction);

  //connect(m_myAction, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_myAction, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

// vp log startup
  std::string path = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toStdString();
  path += "/log/externalplugin.log";

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  std::string layout = "%d{ISO8601} [%t] %-5p %c - %m%n";
  log4cxx::LogString lString(layout.begin(), layout.end());

  log4cxx::FileAppender* fileAppender = new log4cxx::RollingFileAppender(log4cxx::LayoutPtr(new log4cxx::PatternLayout(lString)),
    log4cxx::helpers::Transcoder::decode(path.c_str()), true);

  log4cxx::helpers::Pool p;
  fileAppender->activateOptions(p);

  log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(fileAppender));
  log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getDebug());

  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("vp");
  logger->setAdditivity(false);
  logger->addAppender(fileAppender);

#endif
  m_initialized = true;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
}

void ep::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_myAction;

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  log4cxx::LogManager::shutdown();
#endif

  //TE_LOG_TRACE(TE_TR("TerraLib Qt VP Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void ep::Plugin::onActionActivated(bool checked)
{
  /*std::auto_ptr<ep::qt::MyDialog> dlg(new ep::qt::MyDialog());

  dlg->exec();*/

  std::auto_ptr<ep::qt::RasterFile> dlg(new ep::qt::RasterFile());

  dlg->exec();

}

PLUGIN_CALL_BACK_IMPL(ep::Plugin)


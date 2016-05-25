#ifndef __EXTERNALPLUGIN_PLUGIN_H
#define __EXTERNALPLUGIN_PLUGIN_H

#include "Config.h"

// TerraLib
//#include <terralib/plugin/Config.h>
#include <terralib/plugin/Plugin.h>

// Qt
#include <QAction>

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        struct Event;
      }
    }
  }
}

namespace ep
{
  class Plugin : public QObject, public te::plugin::Plugin
  {
    Q_OBJECT

  public:

    Plugin(const te::plugin::PluginInfo& pluginInfo);

    ~Plugin();

    void startup();

    void shutdown();

  protected slots:

    void onActionActivated(bool checked);

  Q_SIGNALS:

    void triggered(te::qt::af::evt::Event* e);

  protected:

    QAction* m_myAction;

  };
}

PLUGIN_CALL_BACK_DECLARATION(EXTERNALPLUGINEXPORT);

#endif //__EXTERNALPLUGIN_PLUGIN_H

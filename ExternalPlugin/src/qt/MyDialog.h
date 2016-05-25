#ifndef __EXTERNALPLUGIN_QT_MYDIALOG
#define __EXTERNALPLUGIN_QT_MYDIALOG

// Qt
#include <QDialog>

// STL
#include <memory>

namespace Ui { class MyDialogForm; }

namespace ep
{
  namespace qt
  {
    class MyDialog : public QDialog
    {
      Q_OBJECT

      public:

        MyDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
         ~MyDialog();

      protected slots:
  
        void onOkPushButtonClicked();

      private:

      std::auto_ptr<Ui::MyDialogForm> m_ui;

    };
  }
}

#endif  // __EXTERNALPLUGIN_QT_MYDIALOG


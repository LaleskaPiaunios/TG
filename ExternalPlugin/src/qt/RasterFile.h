#ifndef __EXTERNALPLUGIN_QT_RASTERFILE
#define __EXTERNALPLUGIN_QT_RASTERFILE

// Qt
#include <QDialog>

// STL
#include <memory>

// GEOS
#include <../../terralib5/src/terralib/geometry/Geometry.h>
#include <../../terralib5/src/terralib/dataaccess/dataset/DataSet.h>
#include <../../terralib5/src/terralib/dataaccess/datasource/DataSource.h>
#include <../../terralib5/src/terralib/dataaccess/dataset/DataSetTypeConverter.h>
#include "../../terralib5/src/terralib/memory/DataSet.h"
#include "../../terralib5/src/terralib/memory/DataSetItem.h"
#include "CloudDetection.h"

namespace Ui { class RasterFile; }

namespace ep
{
  namespace qt
  {
    class RasterFile : public QDialog
    {
      Q_OBJECT

      public:

        RasterFile(QWidget* parent = 0, Qt::WindowFlags f = 0);
        ~RasterFile();
        
      te::gm::Geometry* dataSetToGeometryCollection(std::auto_ptr<te::da::DataSet> dataSet, te::rst::Raster* raster);

      te::rst::Raster* openImage(const std::string& image);

      std::auto_ptr<te::da::DataSource> openShape(const std::string& shape);

      te::rst::Raster* createOutput(const std::string& output, te::rst::Raster* inraster);

      std::auto_ptr<te::da::DataSet> createDataSetGeometry(std::auto_ptr<te::da::DataSource> dataSource);

      te::gm::MultiPolygon* createMultipolygonDiff(te::gm::Geometry* inGeomColl, te::gm::Geometry* auxGeomColl);

      void cleanRaster(te::rst::Raster* outraster, te::gm::Geometry* inGeomColl, const te::rst::Raster &auxRaster, te::gm::Geometry* auxGeomColl, te::gm::MultiPolygon* m_polygon);

      void copyPixels(const te::rst::Raster& raster, te::gm::MultiPolygon* m_polygon, te::rst::Raster* outraster);

      void copyPixels(const te::rst::Raster& raster, te::gm::Polygon* polygon, te::rst::Raster* outraster);

      std::auto_ptr<te::da::DataSetType> ep::qt::RasterFile::createOutputDataSetType(int srid);

      bool  persistDataSet(te::mem::DataSet* dataset, std::string rasterName, te::gm::Geometry* geom);

      te::da::DataSourcePtr createDataSource();

      void init();

      void generateDetection(std::string name);

      protected slots:
  
      void onPushButtonImageClicked();

      void onPushButtonRemoveImageClicked();

      void onPushButtonOKClicked();

      void onPushButtonOpenImageClicked();

      private:

      std::auto_ptr<Ui::RasterFile> m_ui;

      CloudDetection m_detection;

    };
  }
}

#endif  // __EXTERNALPLUGIN_QT_MYDIALOG


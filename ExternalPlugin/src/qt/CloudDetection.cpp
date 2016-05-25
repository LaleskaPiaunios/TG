#include "CloudDetection.h"
#include "../../terralib5/src/terralib/raster/Raster.h"
#include "../../terralib5/src/terralib/raster/RasterFactory.h"
#include "../../terralib5/src/terralib/raster/Utils.h"
#include "../../terralib5/src/terralib/dataaccess/datasource/DataSource.h"
#include "../../terralib5/src/terralib/dataaccess/datasource/DataSourceFactory.h"
#include "../../terralib5/src/terralib/dataaccess/datasource/DataSourceTransactor.h"
#include <../../terralib5/src/terralib/dataaccess/dataset/DataSetTypeConverter.h>
#include "../../terralib5/src/terralib/memory/DataSet.h"
#include "../../terralib5/src/terralib/memory/DataSetItem.h"
#include "../../terralib5/src/terralib/vp/Utils.h"
#include "../../terralib5/src/terralib/geometry/GeometryProperty.h"
#include "../../terralib5/src/terralib/rp/Filter.h"

CloudDetection::CloudDetection()
{
  //generateLimiarForCloudRaster();
  //generateLimiarForShadowRaster();
  //openSum();
  //te::rst::Raster* rErosion = generateFilterErosion();
  //te::rst::Raster* rDilation = generateFilterDilation(rErosion);
  //generateVector(rDilation);

  //std::map<std::string, std::string> rinfo;
  //rinfo["URI"] = "D:/Desktop/TG/RESULTADO_NEWEPSG4291/FilterDilation.tif";
  //te::rst::Raster* rasterDilation = te::rst::RasterFactory::open(rinfo);
  //createShape(rDilation);
}

CloudDetection::~CloudDetection()
{

}

void CloudDetection::openSum(std::string& name)
{
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = name + "_limiarCloud.tif";

  te::rst::Raster* rasterCloud = te::rst::RasterFactory::open(rinfo);

  std::map<std::string, std::string> irinfo;
  irinfo["URI"] = name + "_limiarShadow.tif";

  te::rst::Raster* rasterShadow = te::rst::RasterFactory::open(irinfo);

  te::rst::Raster* rasterOut = te::rst::RasterFactory::make();

  generateSumRasters(rasterCloud, rasterShadow, rasterOut, name);
}

te::rst::Raster* CloudDetection::generateFilterErosion(std::string& name)
{
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = name + "_sum.tif";
  te::rst::Raster* raster = te::rst::RasterFactory::open(rinfo);

  // Creating the algorithm parameters
  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::DilationFilterT;

  algoInputParams.m_inRasterPtr = raster;

  algoInputParams.m_inRasterBands.push_back(0);

  algoInputParams.m_iterationsNumber = 1;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = name + "_filterErosion.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm
  te::rp::Filter algorithmInstance;
  if (!algorithmInstance.initialize(algoInputParams))
  {
    std::cout << "Algorithm initialization error." << std::endl;
    return 0;
  }

  if (!algorithmInstance.execute(algoOutputParams))
  {
    std::cout << "Algorithm execution error." << std::endl;
    return 0;
  }
  else
  {
    std::cout << "Algorithm execution OK." << std::endl;
  }

  te::rst::Raster* rOut = algoOutputParams.m_outputRasterPtr.release();

  return rOut;
  delete rOut;

}

te::rst::Raster* CloudDetection::generateFilterDilation(te::rst::Raster* raster, std::string& name)
{
  // Creating the algorithm parameters
  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::ErosionFilterT;

  algoInputParams.m_inRasterPtr = raster;

  algoInputParams.m_inRasterBands.push_back(0);

  algoInputParams.m_iterationsNumber = 1;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = name + "_filterDilation.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm
  te::rp::Filter algorithmInstance;
  if (!algorithmInstance.initialize(algoInputParams))
  {
    std::cout << "Algorithm initialization error." << std::endl;
    return 0;
  }

  if (!algorithmInstance.execute(algoOutputParams))
  {
    std::cout << "Algorithm execution error." << std::endl;
    return 0;
  }
  else
  {
    std::cout << "Algorithm execution OK." << std::endl;
  }

  te::rst::Raster* rOut = algoOutputParams.m_outputRasterPtr.release();

  return rOut;
  delete rOut;

}

bool CloudDetection::generateLimiarForCloudRaster(std::string& name)
{
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = name;

  te::rst::Raster * raster = te::rst::RasterFactory::open(rinfo);

  std::map<std::string, std::string> irinfo;
  irinfo["URI"] = name + "_limiarCloud.tif";

  te::rst::Grid* g = new te::rst::Grid(*raster->getGrid());

  std::vector<te::rst::BandProperty*> bp;

  bp.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

  te::rst::Raster* rasterOut = te::rst::RasterFactory::make("GDAL", g, bp, irinfo);

  te::rst::FillRaster(rasterOut, 255.);
  
  int nRows = raster->getNumberOfRows();
  int nCols = raster->getNumberOfColumns();


  for (int i = 0; i < nRows; ++i)
  {
    for (int j = 0; j < nCols; ++j)
    {
        double value;

        raster->getValue(j, i, value, 2);

        if ((value >= 95. && value <= 255.))
        {
          rasterOut->setValue(j, i, 0.0);
        }
        else
        {
          rasterOut->setValue(j, i, 255.);
        }
    }
  }
  delete rasterOut;
  return true;
}

bool CloudDetection::generateLimiarForShadowRaster(std::string& name)
{
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = name;

  te::rst::Raster* raster = te::rst::RasterFactory::open(rinfo);

  te::rst::Raster* rasterOut;

  std::map<std::string, std::string> irinfo;
  irinfo["URI"] = name + "_limiarShadow.tif";

  te::rst::Grid* g = new te::rst::Grid(*raster->getGrid());

  std::vector<te::rst::BandProperty*> bp;

  bp.push_back(new te::rst::BandProperty(1, te::dt::UCHAR_TYPE));

  rasterOut = te::rst::RasterFactory::make("GDAL", g, bp, irinfo);

  te::rst::FillRaster(rasterOut, 255.);
  
  int nRows = raster->getNumberOfRows();
  int nCols = raster->getNumberOfColumns();

  for (int i = 0; i < nRows; ++i)
  {
    for (int j = 0; j < nCols; ++j)
    {
      double value;

      raster->getValue(j, i, value, 0);

      if (value <= 50.)
      {
        if (value == 0.)
          rasterOut->setValue(j, i, 255.);
        else
          rasterOut->setValue(j, i, 0.);
      }
      else
      {
        rasterOut->setValue(j, i, 255.);
      }
    }
  }
  delete rasterOut;

  return true;
}

bool CloudDetection::generateSumRasters(te::rst::Raster* rasterCloud, te::rst::Raster* rasterShadow, te::rst::Raster*& rasterOut, std::string& name)
{

  std::size_t found = name.find_last_of(".");;
  name = name.substr(0, found - 3);
  std::map<std::string, std::string> irinfo;
  irinfo["URI"] = name + "_sum.tif";

  te::rst::Grid* g = new te::rst::Grid(*rasterCloud->getGrid());

  std::vector<te::rst::BandProperty*> bp;

  te::rst::BandProperty* b = new te::rst::BandProperty(1, te::dt::UCHAR_TYPE);
  b->m_noDataValue = 255.;
  bp.push_back(b);

  rasterOut = te::rst::RasterFactory::make("GDAL", g, bp, irinfo);

  te::rst::FillRaster(rasterOut, 255.);

  int nCols = rasterOut->getNumberOfColumns();
  int nRows = rasterOut->getNumberOfRows();

  for (int i = 0; i < nCols; ++i)
  {
    for (int j = 0; j < nRows; ++j)
    {

      double valueNuvem;
      double valueSombra;

      rasterCloud->getValue(i, j, valueNuvem);
      if (valueNuvem == 255.)
      {
        rasterShadow->getValue(i, j, valueSombra);
        rasterOut->setValue(i, j, valueSombra);
      }
      else
      {
        rasterOut->setValue(i, j, valueNuvem);
      }
    }
  }
  delete rasterOut;
  return true;
}

std::vector<te::gm::Geometry*> CloudDetection::generateVector(te::rst::Raster* raster)
{
  std::vector<te::gm::Geometry*> geoms;
  raster->vectorize(geoms, 0);
  return geoms;
}


te::da::DataSourcePtr CloudDetection::createDataSource(std::string& name)
{
  //Criando o datasource
  std::string filename(name + "_shapeDetection.shp");

  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR"));
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();

  return srcDs;
}



std::auto_ptr<te::da::DataSetType> CloudDetection::createOutputDataSetType(int srid, std::string& name)
{
  std::auto_ptr<te::da::DataSetType> outDsType(new te::da::DataSetType(name));
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("GEOMETRY");
  geometry->setGeometryType(te::gm::PolygonType);
  geometry->setSRID(srid);
  outDsType->add(geometry);
  return outDsType;
}

void CloudDetection::createShape(te::rst::Raster* raster, std::string& name)
{
  std::size_t found = name.find_last_of(".");;
  name = name.substr(0, found - 3);
  te::da::DataSourcePtr srcDs = createDataSource(name);
  std::auto_ptr<te::da::DataSetType> dataSetType = createOutputDataSetType(raster->getSRID(), name + "_shapeDetection");

  std::auto_ptr<te::mem::DataSet> dataset(new te::mem::DataSet(dataSetType.get()));

  std::vector<te::gm::Geometry*> geoms = generateVector(raster);

  persistDataSet(dataset.get(), geoms);

  te::vp::Save(srcDs.get(), dataset.get(), dataSetType.get());
}

void CloudDetection::persistDataSet(te::mem::DataSet* dataset, std::vector<te::gm::Geometry*> geoms)
{
  for (std::size_t i = 0; i < geoms.size(); i++)
  {
    //Criando um item para cada registro
    std::auto_ptr<te::mem::DataSetItem> outDSetItem(new te::mem::DataSetItem(dataset));
    outDSetItem->setGeometry("GEOMETRY", geoms[i]);
    dataset->add(outDSetItem.release());
  }
}
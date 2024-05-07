#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Light>
#include <osg/LightSource>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

int main(int argc, char* argv[])
{
    // Crear un geode para contener nuestra forma
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    // Crear una forma de cubo
    osg::ref_ptr<osg::Box> cube = new osg::Box(osg::Vec3(), 1.0f);

    // Crear un drawable para mostrar el cubo
    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(cube);

    // Agregar material al cubo
    osg::ref_ptr<osg::Material> material = new osg::Material;
    material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Establecer el color del material

    // Asociar el material con el drawable
    shapeDrawable->getOrCreateStateSet()->setAttribute(material.get());

    // Adjuntar el drawable al geode
    geode->addDrawable(shapeDrawable);

    // Crear una transformación de matriz para contener el geode
    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;

    // Agregar el geode a la transformación de matriz
    root->addChild(geode);

    // Establecer la posición inicial del cubo
    osg::Matrix initialMatrix;
    initialMatrix.makeTranslate(0.0f, 0.0f, 5.0f);
    root->setMatrix(initialMatrix);

    // Crear un visor
    osgViewer::Viewer viewer;

    // Configurar la manipulación de la vista para permitir el arrastre
    viewer.setCameraManipulator(new osgGA::TrackballManipulator); // Utilizamos el TrackballManipulator

    // Establecer los datos de la escena
    viewer.setSceneData(root);

    // Crear una luz predeterminada
    osg::ref_ptr<osg::Light> light = new osg::Light;
    light->setLightNum(0); // Establecer el número de luz
    light->setPosition(osg::Vec4(2.0f, 0.0f, 1.0f, 0.0f)); // Establecer la posición de la luz
    light->setAmbient(osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f)); // Establecer el color de la luz ambiental
    light->setDiffuse(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f)); // Establecer el color de la luz difusa
    light->setSpecular(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Establecer el color de la luz especular

    // Crear una fuente de luz y agregar la luz a ella
    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
    lightSource->setLight(light);

    // Agregar la fuente de luz al nodo raíz
    root->addChild(lightSource);

    // Obtener el tiempo inicial para hacer girar el cubo
    double startTime = osg::Timer::instance()->tick();

    while (!viewer.done())
    {
        // Obtener el tiempo actual
        double currentTime = osg::Timer::instance()->delta_s(startTime, osg::Timer::instance()->tick());

        // Obtener la matriz de transformación del nodo Geode
        osg::Matrix rotationMatrix;
        rotationMatrix.makeRotate(osg::Quat(currentTime, osg::Vec3(2.0, 1.5, 1.0))); // Rotación sobre el eje Z con el tiempo

        // Aplicar la rotación a la matriz de transformación del nodo MatrixTransform
        root->setMatrix(rotationMatrix);

        // Actualizar la escena
        viewer.frame();
    }
    return 0;
}


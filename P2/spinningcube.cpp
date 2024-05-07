#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/PositionAttitudeTransform>

// Create a callback to rotate the cube
class RotateCB : public osg::NodeCallback
{
	public: float rotationx=0;
	public: float rotationy=0;
	public: float rotationz=0;
	private: osg::ref_ptr<osg::AnimationPathCallback> animationCallback;

	public: RotateCB(float x, float y, float z){
		rotationx=x;
		rotationy=y;
		rotationz=z;

		// Crear una ruta de animación
		osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
		animationPath->setLoopMode(osg::AnimationPath::LOOP);

		// Add control points to the animation path
		double animationTime = 0.0;
		for (int i = 0; i < 10; ++i) { // 10 random positions
			osg::Vec3 position(-2.0 + rand() % 5, 
							-2.0 + rand() % 5, 
							-2.0 + rand() % 5); // Generate a random position
			animationPath->insert(animationTime, osg::AnimationPath::ControlPoint(position));
			animationTime += (rand() % 6)+1; // Move to the next position after 5 seconds
		}

		// Create an animation path callback and add it to the position transform node
		animationCallback = new osg::AnimationPathCallback(animationPath);
	}
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		animationCallback.get()->operator()(node,nv);

		osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*>(node);
		double angle = osg::PI * ( (osg::Timer::instance()->time_s()) / 10.0 );
		pat->setAttitude(osg::Quat(angle, osg::Vec3(rotationx, rotationy, rotationz)));
		traverse(node,nv);
	}
};

int main()
{
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0,0,0),1.0f);
    osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(boxDrawable);

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setMatrix(osg::Matrix::translate(0,0,0)); // Offset the cube by its center
	mt->addChild(geode);

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	

    pat->addChild(mt);
    pat->setUpdateCallback(new RotateCB(0.2,0,1));

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(pat);

    // Create another cube
	osg::ref_ptr<osg::Box> box2 = new osg::Box(osg::Vec3(0,0,0),1.0f); // Positioned at (3,0,0)
	osg::ref_ptr<osg::ShapeDrawable> boxDrawable2 = new osg::ShapeDrawable(box2);
	osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
	geode2->addDrawable(boxDrawable2);

	osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform;
	mt2->setMatrix(osg::Matrix::translate(3,0,0)); // Offset the cube by its center
	mt2->addChild(geode2);

	osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform();
	pat2->addChild(mt2);
	pat2->setPivotPoint(osg::Vec3(3,0,0));
	pat2->setPosition(osg::Vec3(3,0,0));
	pat2->setUpdateCallback(new RotateCB(1,0.5,0));

	root->addChild(pat2);

	// Create a color array
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> colors2 = new osg::Vec4Array;

	// Get the Geometry of the Geodes
	osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode->getDrawable(0));
	osg::Geometry* geometry2 = dynamic_cast<osg::Geometry*>(geode2->getDrawable(0));

	// Add a color to the array
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); // White color
	colors2->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue color
	// Set the color array on the Geometry
	geometry->setColorArray(colors);
	geometry2->setColorArray(colors2);

	// Set the color binding to use the first color of the array for all vertices
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	geometry2->setColorBinding(osg::Geometry::BIND_OVERALL);


	// Create a StateSet at the root node
	osg::StateSet* stateSet = root->getOrCreateStateSet();

	// Enable lighting
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	// Enable lighting
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	// Enable light number 0
	stateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);

	// Crear un nuevo cubo luz
	osg::ref_ptr<osg::Box> boxL = new osg::Box(osg::Vec3(0, 0, 0), 0.1f);
	osg::ref_ptr<osg::ShapeDrawable> boxDrawableL = new osg::ShapeDrawable(boxL);
	osg::ref_ptr<osg::Geode> boxGeodeL = new osg::Geode;
	boxGeodeL->addDrawable(boxDrawableL.get());

	// Crear un nodo de transformación de posición y actitud
	osg::ref_ptr<osg::PositionAttitudeTransform> boxPatL = new osg::PositionAttitudeTransform;

	// Convert the Vec4 to a Vec3
	osg::Vec3 lightPosition3(osg::Vec3(0,0,0));

	// Set the position of the cube to match the light position
	boxPatL->setPosition(lightPosition3);

	// Agregar el Geode al nodo de transformación de posición y actitud
	boxPatL->addChild(boxGeodeL.get());

    // Enable light number 1
	stateSet->setMode(GL_LIGHT1, osg::StateAttribute::ON);

	// Crear una luz
	osg::ref_ptr<osg::Light> light = new osg::Light;

    // Configurar la luz
	light->setLightNum(1); // Número de la luz, debe ser único
	light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0)); // Posición de la luz, el último parámetro es 1.0 para una luz puntual
	light->setDiffuse(osg::Vec4(1.0, 0.0, 0.5, 1.0)); // Color difuso de la luz
	light->setSpecular(osg::Vec4(1.0, 0.0, 0.5, 1.0)); // Color especular de la luz

	// Crear un nodo de luz y agregar la luz al nodo
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
	lightSource->setLight(light);

	// Agregar el nodo de luz al nodo raíz
	boxPatL->addChild(lightSource);


	// Agregar el nodo de transformación de posición y actitud al nodo raíz
	root->addChild(boxPatL.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.run();
}

/*  This code creates a cube and adds it to a PositionAttitudeTransform node. The RotateCB class is a callback that rotates the cube around the z-axis. The rotation speed is set to one full rotation every 10 seconds. The callback is added to the PositionAttitudeTransform node, which applies the rotation to the cube. The cube is then added to the scene and displayed in the viewer. */


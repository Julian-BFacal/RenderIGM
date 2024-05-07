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

	// Create a color array
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;


	// Get the Geometry of the Geodes
	osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode->getDrawable(0));

	// Add a color to the array
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); // White color
	// Set the color array on the Geometry
	geometry->setColorArray(colors);

	// Set the color binding to use the first color of the array for all vertices
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	// Create a StateSet at the root node
	osg::StateSet* stateSet = root->getOrCreateStateSet();

	// Enable lighting
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	// Enable light number 0
	stateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);

    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.run();
}

/*  This code creates a cube and adds it to a PositionAttitudeTransform node. The RotateCB class is a callback that rotates the cube around the z-axis. The rotation speed is set to one full rotation every 10 seconds. The callback is added to the PositionAttitudeTransform node, which applies the rotation to the cube. The cube is then added to the scene and displayed in the viewer. */


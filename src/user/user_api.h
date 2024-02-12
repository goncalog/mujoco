// Copyright 2024 DeepMind Technologies Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MUJOCO_SRC_USER_USER_API_H_
#define MUJOCO_SRC_USER_USER_API_H_

#include <mujoco/mjexport.h>
#include <mujoco/mjmodel.h>
#include <mujoco/mjtnum.h>


// this is a C-API
#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------- handles to internal objects -----------------------------------

typedef struct _mjElement* mjElement;
typedef struct _mjString* mjString;
typedef struct _mjDouble* mjDouble;


//---------------------------------- enum types (mjt) ----------------------------------------------

typedef enum _mjtGeomInertia {     // type of inertia inference
  mjINERTIA_VOLUME,                // mass distributed in the volume
  mjINERTIA_SHELL,                 // mass distributed on the surface
} mjtGeomInertia;


//---------------------------------- attribute structs (mjm) ---------------------------------------

typedef struct _mjmOrientation {   // alternative orientation specifiers
  double axisangle[4];             // rotation axis and angle
  double xyaxes[6];                // x and y axes
  double zaxis[3];                 // z axis (use minimal rotation)
  double euler[3];                 // euler angles
} mjmOrientation;


typedef struct _mjmPlugin {        // plugin specification
  mjElement instance;              // internal, do not modify
  mjString name;                   // name
  mjString instance_name;          // instance name
  bool active;                     // is the plugin active
} mjmPlugin;


typedef struct _mjmBody {          // body specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // childclass name

  // body frame
  double pos[3];                   // frame position
  double quat[4];                  // frame orientation
  mjmOrientation alt;              // frame alternative orientation

  // inertial frame
  double mass;                     // mass
  double ipos[3];                  // inertial frame position
  double iquat[4];                 // inertial frame orientation
  double inertia[3];               // diagonal inertia (in i-frame)
  mjmOrientation ialt;             // inertial frame alternative orientation
  double fullinertia[6];           // non-axis-aligned inertia matrix

  // other
  mjtByte mocap;                   // is this a mocap body
  double gravcomp;                 // gravity compensation
  mjDouble userdata;               // user data
  mjtByte explicitinertial;        // whether to save the body with explicit inertial clause
  mjmPlugin plugin;                // passive force plugin
  mjString info;                   // message appended to compiler errors
} mjmBody;


typedef struct _mjmFrame {         // frame specification
  mjElement element;               // internal, do not modify
  double pos[3];                   // position
  double quat[4];                  // orientation
  mjmOrientation alt;              // alternative orientation
  mjString info;                   // message appended to compiler errors
} mjmFrame;


typedef struct _mjmJoint {         // joint specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name
  mjtJoint type;                   // joint type

  // kinematics
  double pos[3];                   // anchor position
  double axis[3];                  // joint axis
  double ref;                      // value at reference configuration: qpos0

  // stiffness
  double stiffness;                // stiffness coefficient
  double springref;                // spring reference value: qpos_spring
  double springdamper[2];          // timeconst, dampratio

  // limits
  int limited;                     // does joint have limits
  double range[2];                 // joint limits
  double margin;                   // margin value for joint limit detection
  mjtNum solref_limit[mjNREF];     // solver reference: joint limits
  mjtNum solimp_limit[mjNIMP];     // solver impedance: joint limits
  int actfrclimited;               // are actuator forces on joint limited
  double actfrcrange[2];           // actuator force limits

  // dof properties
  double armature;                 // armature inertia (mass for slider)
  double damping;                  // damping coefficient
  double frictionloss;             // friction loss
  mjtNum solref_friction[mjNREF];  // solver reference: dof friction
  mjtNum solimp_friction[mjNIMP];  // solver impedance: dof friction

  // other
  int group;                       // group
  double urdfeffort;               // effort (urdf)
  mjDouble userdata;               // user data
  mjString info;                   // message appended to compiler errors
} mjmJoint;


typedef struct _mjmGeom {          // geom specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // classname
  mjtGeom type;                    // geom type

  // frame, size
  double pos[3];                   // position
  double quat[4];                  // orientation
  mjmOrientation alt;              // alternative orientation
  double fromto[6];                // alternative for capsule, cylinder, box, ellipsoid
  double size[3];                  // type-specific size

  // contact related
  int contype;                     // contact type
  int conaffinity;                 // contact affinity
  int condim;                      // contact dimensionality
  int priority;                    // contact priority
  double friction[3];              // one-sided friction coefficients: slide, roll, spin
  double solmix;                   // solver mixing for contact pairs
  mjtNum solref[mjNREF];           // solver reference
  mjtNum solimp[mjNIMP];           // solver impedance
  double margin;                   // margin for contact detection
  double gap;                      // include in solver if dist < margin-gap

  // inertia inference
  double mass;                     // used to compute density
  double density;                  // used to compute mass and inertia from volume or surface
  mjtGeomInertia typeinertia;      // selects between surface and volume inertia

  // fluid forces
  mjtNum fluid_ellipsoid;          // whether ellipsoid-fluid model is active
  mjtNum fluid_coefs[5];           // ellipsoid-fluid interaction coefs

  // visual
  mjString material;               // name of material
  float rgba[4];                   // rgba when material is omitted
  int group;                       // group

  // other
  mjString hfieldname;             // heightfield attached to geom
  mjString meshname;               // mesh attached to geom
  double fitscale;                 // scale mesh uniformly
  mjDouble userdata;               // user data
  mjmPlugin plugin;                // sdf plugin
  mjString info;                   // message appended to compiler errors
} mjmGeom;


typedef struct _mjmSite {          // site specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name

  // frame, size
  double pos[3];                   // position
  double quat[4];                  // orientation
  mjmOrientation alt;              // alternative orientation
  double fromto[6];                // alternative for capsule, cylinder, box, ellipsoid
  double size[3];                  // geom size

  // visual
  mjtGeom type;                    // geom type
  mjString material;               // name of material
  int group;                       // group
  float rgba[4];                   // rgba when material is omitted

  // other
  mjDouble userdata;               // user data
  mjString info;                   // message appended to compiler errors
} mjmSite;


typedef struct _mjmCamera {        // camera specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name

  // extrinsics
  double pos[3];                   // position
  double quat[4];                  // orientation
  mjmOrientation alt;              // alternative orientation
  mjtCamLight mode;                // tracking mode
  mjString targetbody;             // target body for tracking/targeting

  // intrinsics
  double fovy;                     // y-field of view
  double ipd;                      // inter-pupilary distance
  float intrinsic[4];              // camera intrinsics (length)
  float sensor_size[2];            // sensor size (length)
  float resolution[2];             // resolution (pixel)
  float focal_length[2];           // focal length (length)
  float focal_pixel[2];            // focal length (pixel)
  float principal_length[2];       // principal point (length)
  float principal_pixel[2];        // principal point (pixel)

  // other
  mjDouble userdata;               // user data
  mjString info;                   // message appended to compiler errors
} mjmCamera;


typedef struct _mjmLight {         // light specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name

  // frame
  double pos[3];                   // position
  double dir[3];                   // direction
  mjtCamLight mode;                // tracking mode
  mjString targetbody;             // target body for targeting

  // intrinsics
  mjtByte active;                  // is light active
  mjtByte directional;             // is light directional or spot
  mjtByte castshadow;              // does light cast shadows
  float attenuation[3];            // OpenGL attenuation (quadratic model)
  float cutoff;                    // OpenGL cutoff
  float exponent;                  // OpenGL exponent
  float ambient[3];                // ambient color
  float diffuse[3];                // diffuse color
  float specular[3];               // specular color

  // other
  mjString info;                   // message appended to compiler errors
} mjmLight;


typedef struct _mjmMaterial {      // material specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name
  mjString texture;                // name of texture (empty: none)
  bool texuniform;                 // make texture cube uniform
  float texrepeat[2];              // texture repetition for 2D mapping
  float emission;                  // emission
  float specular;                  // specular
  float shininess;                 // shininess
  float reflectance;               // reflectance
  float rgba[4];                   // rgba
  mjString info;                   // message appended to compiler errors
} mjmMaterial;


typedef struct _mjmEquality {      // equality specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name
  mjtEq type;                      // constraint type
  double data[mjNEQDATA];          // type-dependent data
  mjtByte active;                  // is equality initially active
  mjString name1;                  // name of object 1
  mjString name2;                  // name of object 2
  mjtNum solref[mjNREF];           // solver reference
  mjtNum solimp[mjNIMP];           // solver impedance
  mjString info;                   // message appended to errors
} mjmEquality;


typedef struct _mjmTendon {        // tendon specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name

  // stiffness, damping, friction
  double stiffness;                // stiffness coefficient
  double springlength[2];          // spring resting length; {-1, -1}: use qpos_spring
  double damping;                  // damping coefficient
  double frictionloss;             // friction loss
  mjtNum solref_friction[mjNREF];  // solver reference: tendon friction
  mjtNum solimp_friction[mjNIMP];  // solver impedance: tendon friction

  // length range
  int limited;                     // does tendon have limits: 0 false, 1 true, 2 auto
  double range[2];                 // length limits
  double margin;                   // margin value for tendon limit detection
  mjtNum solref_limit[mjNREF];     // solver reference: tendon limits
  mjtNum solimp_limit[mjNIMP];     // solver impedance: tendon limits

  // visual
  mjString material;               // name of material for rendering
  double width;                    // width for rendering
  float rgba[4];                   // rgba when material is omitted
  int group;                       // group

  // other
  mjDouble userdata;               // user data
  mjString info;                   // message appended to errors
} mjmTendon;


typedef struct _mjmWrap {          // wrapping object specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name
  mjString info;                   // message appended to errors
} mjmWrap;


typedef struct _mjmActuator {      // actuator specification
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name

  // gain, bias
  mjtGain gaintype;                // gain type
  double gainprm[mjNGAIN];         // gain parameters
  mjtBias biastype;                // bias type
  double biasprm[mjNGAIN];         // bias parameters

  // activation state
  mjtDyn dyntype;                  // dynamics type
  double dynprm[mjNDYN];           // dynamics parameters
  int actdim;                      // number of activation variables
  int plugin_actdim;               // actuator state size for plugins
  mjtByte actearly;                // apply next activations to qfrc

  // transmission
  mjtTrn trntype;                  // transmission type
  double gear[6];                  // length and transmitted force scaling
  mjString target;                 // name of transmission target
  mjString refsite;                // reference site, for site transmission
  mjString slidersite;             // site defining cylinder, for slider-crank
  double cranklength;              // crank length, for slider-crank
  double lengthrange[2];           // transmission length range

  // input/output clamping
  int ctrllimited;                 // are control limits defined: 0 false, 1 true, 2 auto
  double ctrlrange[2];             // control range
  int forcelimited;                // are force limits defined: 0 false, 1 true, 2 auto
  double forcerange[2];            // force range
  int actlimited;                  // are activation limits defined: 0 false, 1 true, 2 auto
  double actrange[2];              // activation range

  // other
  int group;                       // group
  mjDouble userdata;               // user data
  mjmPlugin plugin;                // actuator plugin
  mjString info;                   // message appended to compiler errors
} mjmActuator;


typedef struct _mjmSensor {        // sensor specfication
  mjElement element;               // internal, do not modify
  mjString name;                   // name
  mjString classname;              // class name

  // sensor defintion
  mjtSensor type;                  // type of sensor
  mjtObj objtype;                  // type of sensorized object
  mjString objname;                // name of sensorized object
  mjtObj reftype;                  // type of referenced object
  mjString refname;                // name of referenced object

  // user-defined sensors
  mjtDataType datatype;            // data type for sensor measurement
  mjtStage needstage;              // compute stage needed to simulate sensor
  int dim;                         // number of scalar outputs

  // output post-processing
  double cutoff;                   // cutoff for real and positive datatypes
  double noise;                    // noise stdev

  // other
  mjDouble userdata;               // user data
  mjmPlugin plugin;                // sensor plugin
  mjString info;                   // message appended to compiler errors
} mjmSensor;


//---------------------------------- API functions -------------------------------------------------

// Create model.
MJAPI void* mjm_createModel();

// Delete model.
MJAPI void mjm_deleteModel(void* modelspec);

// Copy spec into private attributes.
MJAPI void mjm_finalize(mjElement object);

// Add child body to body, return child spec.
MJAPI mjmBody* mjm_addBody(mjmBody* body, void* defspec);

// Add site to body, return site spec.
MJAPI mjmSite* mjm_addSite(mjmBody* body, void* defspec);

// Add joint to body.
MJAPI mjmJoint* mjm_addJoint(mjmBody* body, void* defspec);

// Add freejoint to body.
MJAPI mjmJoint* mjm_addFreeJoint(mjmBody* body);

// Add geom to body.
MJAPI mjmGeom* mjm_addGeom(mjmBody* body, void* defspec);

// Add camera to body.
MJAPI mjmCamera* mjm_addCamera(mjmBody* body, void* defspec);

// Add light to body.
MJAPI mjmLight* mjm_addLight(mjmBody* body, void* defspec);

// Add frame to body.
MJAPI mjmFrame* mjm_addFrame(mjmBody* body, mjmFrame* parentframe);

// Add material to model.
MJAPI mjmMaterial* mjm_addMaterial(void* model, void* defspec);

// Add equality to model.
MJAPI mjmEquality* mjm_addEquality(void* model, void* defspec);

// Add tendon to model.
MJAPI mjmTendon* mjm_addTendon(void* model, void* defspec);

// Wrap site using tendon.
MJAPI mjmWrap* mjm_wrapSite(mjmTendon* tendon, const char* name);

// Wrap geom using tendon.
MJAPI mjmWrap* mjm_wrapGeom(mjmTendon* tendon, const char* name, const char* sidesite);

// Wrap joint using tendon.
MJAPI mjmWrap* mjm_wrapJoint(mjmTendon* tendon, const char* name, double coef);

// Wrap pulley using tendon.
MJAPI mjmWrap* mjm_wrapPulley(mjmTendon* tendon, double divisor);

// Add actuator to model.
MJAPI mjmActuator* mjm_addActuator(void* model, void* defspec);

// Add sensor to model.
MJAPI mjmSensor* mjm_addSensor(void* model);

// Add plugin to model.
MJAPI mjElement mjm_addPlugin(void* model);

// Get model from body.
MJAPI void* mjm_getModel(mjmBody* body);

// Get default corresponding to an mjElement.
MJAPI void* mjm_getDefault(mjElement element);

// Find body in model by name.
MJAPI mjmBody* mjm_findBody(void* modelspec, const char* name);

// Find child body by name.
MJAPI mjmBody* mjm_findChild(mjmBody* body, const char* name);

// Get element id.
MJAPI int mjm_getId(mjElement element);

// Copy text to destination string.
MJAPI void mjm_setString(mjString dest, const char* text);

// Copy double array to destination vector.
MJAPI void mjm_setDouble(mjDouble dest, const double* array, int size);

// Get string contents.
MJAPI const char* mjm_getString(mjString source);

// Get double array contents and optionally its size.
MJAPI const double* mjm_getDouble(mjDouble source, int* size);

// Set default.
MJAPI void mjm_setDefault(mjElement element, void* defspec);

// Set frame.
MJAPI void mjm_setFrame(mjElement dest, mjmFrame* frame);

// Compute quat and inertia from body->fullinertia.
MJAPI const char* mjm_setFullInertia(mjmBody* body, double quat[4], double inertia[3]);


//---------------------------------- Initialization functions --------------------------------------

// Default body attributes.
MJAPI void mjm_defaultBody(mjmBody& body);

// Default frame attributes.
MJAPI void mjm_defaultFrame(mjmFrame& frame);

// Default joint attributes.
MJAPI void mjm_defaultJoint(mjmJoint& joint);

// Default geom attributes.
MJAPI void mjm_defaultGeom(mjmGeom& geom);

// Default site attributes.
MJAPI void mjm_defaultSite(mjmSite& site);

// Default camera attributes.
MJAPI void mjm_defaultCamera(mjmCamera& camera);

// Default light attributes.
MJAPI void mjm_defaultLight(mjmLight& light);

// Default material attributes.
MJAPI void mjm_defaultMaterial(mjmMaterial& material);

// Default equality attributes.
MJAPI void mjm_defaultEquality(mjmEquality& equality);

// Default tendon attributes.
MJAPI void mjm_defaultTendon(mjmTendon& tendon);

// Default actuator attributes.
MJAPI void mjm_defaultActuator(mjmActuator& actuator);

// Default sensor attributes.
MJAPI void mjm_defaultSensor(mjmSensor& sensor);

#ifdef __cplusplus
}
#endif

#endif  // MUJOCO_SRC_USER_USER_API_H_
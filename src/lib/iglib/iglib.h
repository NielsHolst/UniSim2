namespace ig {

enum Origin {UserDefined, Measured, NotAvailable};

struct Variable {
    double value;
    Origin origin;
};

struct TimeStamp {
    int dayOfYear;	    // 1..365
    double 
        timeOfDay,	    // 0..24
  		timeZone;		// -24..24 (relative to UTC)
};

struct Greenhouse {
	double 
		latitude,
		longitude,
        direction;		// m
};

enum CultureType {PotRose, PotChrysanthemum, Cucumber, Tomato, Lettuce, SweetPepper};
struct Culture {
    CultureType type;
    Variable lai;
};

struct CoverMaterial {
    double
        emissivity,                 // 0..1
        absorptivity,               // 0..1
        transmissivity,             // 0..1
        haze,                       // 0..1
        U,                          // W/m2/K
        heatCapacity;               // J/m2/K
};

struct FloorMaterial {
    double
        emissivity,                 // 0..1
        Uindoors,                   // W/m2/K
        Usoil,                      // W/m2/K
        heatCapacity;               // J/m2/K
};

struct Construction {
	double
        length,			    // m
        spanWidth;          // m
    int spanCount;          // >0
    double
        wallHeight,         // m
        roofInclination,    // 0..360
        internalShading, 	// 0..1
        infiltration;       // h-1
    CoverMaterial
        end1,
        end2,
        side1,
        side2,
        roof1,
        roof2;
    FloorMaterial
        floor;
};

enum HeatPipeMaterial {Iron, Plastic, Aluminium};

struct HeatPipe {
    HeatPipeMaterial material;
    Variable 
        flowRate,           // m3/h
        temperatureInflow,  // oC
        temperatureOutflow, // oC
        innerDiameter,      // mm
        outerDiameter,      // mm
        length;             // m pipe / m floor area
};

struct HeatPipes {
    const HeatPipe *array;
    int size;
};

struct Vent {
    double
        length,              // m
        height,             // m
        numberOfVents,      // >0
        maxOpening,         // 0..180
        porosity;           // 0..1
    Variable opening;       // 0..1
};

struct Vents {
    const Vent *array;
    int size;
};

enum GrowthLightType {Hpsl, Led};
struct GrowthLight {
    GrowthLightType type;
    double 
        intensity,          // W/m2  (installed effect per greenhouse area)
        ballastCorrection;  // >=1 (>1 if intensity includes ballast
    Variable
        age,                // h
        lifeTime,           // h
        on;                 // 0..1
};
struct GrowthLights {
    const GrowthLight *array;
    int size;
};

struct Co2Dispenser {
    Variable injectionRate;    // g/m2/h
};

struct ScreenMaterial {
    double
        transmissivityLight,    // 0..1
        emmisivityInner,        // 0..1
        emmisivityOuter,        // 0..1
        haze,                   // 0..1
        energySaving,           // 0..100 
        transmissivityAir,      // 0..1
        U,                      // W/m2/K
        heatCapacity;           // J/m2/K
};

enum ScreenLayer {Inner, Mid, Outer};
enum ScreenPosition {WholeRoof, FlatRoof, Roof1, Roof2, Side1, Side2, End1, End2};

struct Screen {
    ScreenMaterial material;
    ScreenLayer layer;
    ScreenPosition position;
    Variable effect;                // 0..1
};
struct Screens {
    const Screen *array;
    int size;
};

struct Dehumidifier {
    // ?
};
struct Dehumidifiers {
    Dehumidifier *dehumidifier;
    int size;
};

struct Outdoors {
    Variable
        temperature,            // oC
        irradiation,            // W/m2
        rh,                     // 0..100
        co2,                    // ppm
        windspeed,              // m/s
        windDirection;          // 0..360
};

struct Indoors {
    Variable
        temperature,            // oC
        lightIntensity,         // W/m2
        rh,                     // 0..100
        co2;                    // ppm
};


struct Query {
    TimeStamp timeStamp;
    Greenhouse greenhouse;
    Culture culture;
    Construction construction;
    HeatPipes heatPipes;
    Vents vents;
    GrowthLights growthLights;
    Co2Dispenser co2Dispenser;
    Screens screens;
    Dehumidifiers dehumidifiers;
    Outdoors outdoors;
    Indoors indoors;
};

struct Response {
    TimeStamp timeStamp;
    double
        photosynthesisRate,     // g/m2/h
        photosynthesisPct,      // 0..100 (100 = at light saturation and current temp, CO2 and r.h.)
        lightOutdoors,          // mymol PAR/m2/s
        lightArtificial,        // mymol PAR/m2/s
        lightPlantHeight,       // mymol PAR/m2/s
        co2Indoors;             // ppm (measured or computed)
    ;
    bool hasError;
    const char *error;
};

// Generate a query at random
extern "C" Query __declspec(IGLIB_DLL) randomQuery();

// Compute response variables from query
extern "C" Response __declspec(IGLIB_DLL) compute(const Query &q);

// Convert query to a string presentation
extern "C" const char * __declspec(IGLIB_DLL) queryToString(const Query &q);

// Convert response to a string presentation
extern "C" const char * __declspec(IGLIB_DLL) responseToString(const Response &r);

// Release memory resources; call at the end of the session
extern "C" void __declspec(IGLIB_DLL) release();

} // namespace

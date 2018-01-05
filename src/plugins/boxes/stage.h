    #ifndef STAGE_H
    #define STAGE_H
    #include "stage_base.h"

    namespace boxes {

    class DistributedDelay;

    class Stage : public StageBase
    {
    public:
        Stage(QString name, QObject *parent);
        void myReset();
        void update();

        // special methods
        void createDistributedDelay();
    private:
        // inputs
        double inflow, phaseOutflowProportion;

        // outputs
        double outflow;

        // data
        bool _firstUpdate;
        DistributedDelay *_dd;
    };

    }
    #endif

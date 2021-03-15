#ifndef ABS_RADIO
#define ABS_RADIO

class AbsRadioController {
    public:
    virtual void init();
    virtual LinkedList<Log>* scan();
    virtual void send();

    virtual int getStatsTx();
    virtual int getStatsRx();
};

#endif
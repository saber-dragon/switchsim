#ifndef SWITCHSIM_CROSSBARSCHEDULER_HPP
#define SWITCHSIM_CROSSBARSCHEDULER_HPP

#include <string>
#include <vector>
#include <algorithm>    // std::find
#include "module.hpp"


using namespace std;
namespace saber {
    enum class PortType { in = 0, out = 1 };
    class CrossbarScheduler : public Module {

    protected:
        const int _inputs;// number of inputs
        const int _outputs;// number of outputs

        const bool _outMatchEnabled;// whether or not to enable _outMatch
        vector<int> _inMatch;
        vector<int> _outMatch;

    public:
        static const int free = -1;
        CrossbarScheduler(Module *parent, const string &name,
                          int inputs, int outputs, bool outMatchEnabled = false);

        virtual void Schedule() = 0;
        int MatchedWith(int, const PortType&) const;
    };

    CrossbarScheduler::CrossbarScheduler(Module *parent, const string &name, int inputs, int outputs,
                                         bool outMatchEnabled) :
            Module(parent, name),
            _inputs(inputs),
            _outputs(outputs),
            _outMatchEnabled(outMatchEnabled),
            _inMatch(inputs, free){
            if (_outMatchEnabled) _outMatch(outputs, free);
    }
    int CrossbarScheduler::MatchedWith(int portId, const saber::PortType &portType) const {
        int matched = free;
        switch(portType){
            case PortType::in :
            {
                assert(portId >= 0 && portId < _inputs && "Port ID out of range");
                matched = _inMatch[portId];
                break;
            }
            case PortType::out :
            {
                assert(portId >= 0 && portId < _outputs && "Port ID out of range");
                if (_outMatchEnabled) matched = _outMatch[portId];
                else {
                    auto found = find(_inMatch.begin(), _inMatch.end(), portId);
                    if (found != _inMatch.end()){
                        matched = distance(found, _inMatch.begin());
                    }
                }
                break;
            }
        }
        return matched;
    }
}
#endif //SWITCHSIM_CROSSBARSCHEDULER_HPP

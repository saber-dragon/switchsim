#ifndef SWITCHSIM_MAXIMALSCHEDULER_HPP
#define SWITCHSIM_MAXIMALSCHEDULER_HPP

#include <utility>
#include "crossbarScheduler.hpp"
//#include "random_utils.hpp"

using namespace std;
namespace saber {

    struct Edge {
        int _from;
        int _to;
    };
    class MaximalMatch : public CrossbarScheduler {
    protected:
        vector<Edge> _edges; // all edges
    public:
        MaximalMatch(Module *parent, const string& name,
                     int inputs, int ouputs, bool outMatchEnabled=false);

        virtual void Schedule() = 0;

    };

    MaximalMatch::MaximalMatch(Module *parent, const string &name, int inputs, int ouputs, bool outMatchEnabled) :
    CrossbarScheduler(parent, name, inputs, ouputs, outMatchEnabled){}

    class RandomMaximalMatch : public  MaximalMatch {
    public:
        RandomMaximalMatch(Module *parent, const string& name,
        int inputs, int outputs) :
                MaximalMatch(parent, name, inputs, outputs, true){};

        void Schedule() override ;

    };

    void RandomMaximalMatch::Schedule() {
        _edges.clear();
        for ( int i = 0;i < _inputs;++ i ){
            for ( int j = 0;j < _outputs;++ j ){
                if ( Parent()->QueueLength(i, j) > 0){
                    _edges.emplace_back( i, j);
                }
            }
        }

        random_shuffle( _edges.begin(), _edges.end() );

        int matchSize = 0;
        for (const auto& e : _edges){
            if (_inMatch[e._from] == CrossbarScheduler::free &&
                    _outMatch[e._to] == CrossbarScheduler::free){
                _inMatch[e._from] = e._to;
                _outMatch[e._to] = e._from;
                ++ matchSize;
                if (matchSize == min(_inputs, _outputs)) break;
            }
        }
    }

    class GreedyMaximalMatch : public  MaximalMatch {
        vector<pair<size_t , int> > _weights;
    public:
        GreedyMaximalMatch(Module *parent, const string& name,
                           int inputs, int outputs) :
                MaximalMatch(parent, name, inputs, outputs, true){};

        void Schedule() override ;

    };

    void GreedyMaximalMatch::Schedule() {
        _edges.clear();
        _weights.clear();
        size_t idx = 0;
        for ( int i = 0;i < _inputs;++ i ){
            for ( int j = 0;j < _outputs;++ j ){
                if ( (auto w = Parent()->QueueLength(i, j)) > 0){
                    _edges.emplace_back( i, j);
                    _weights.emplace_back( idx ++, w);
                }
            }
        }

        sort( _weights.begin(), _weights.end(), [](const pair<size_t , int>& a,
        const pair<size_t, int>& b){ return a.second < b.second;} );


        int matchSize = 0;
        for (const auto& w : _weights){
            auto e = _edges[w.first];
            if (_inMatch[e._from] == CrossbarScheduler::free &&
                _outMatch[e._to] == CrossbarScheduler::free){
                _inMatch[e._from] = e._to;
                _outMatch[e._to] = e._from;
                ++ matchSize;
                if (matchSize == min(_inputs, _outputs)) break;
            }
        }
    }
}

#endif //SWITCHSIM_MAXIMALSCHEDULER_HPP

#ifndef __LEADER_AGENT__H
#define __LEADER_AGENT__H 

#include <string>
#include <math.h>
#include "enviro.h"

namespace {

    using namespace enviro;

    // First Stop
    class FirstStop : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}
        void during() { 
            double tolerance = 3.0;
            double xcoor = 250;
            double ycoor = 250;
            move_toward(xcoor, ycoor);

            // Move to next
            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reach the first stop!";
                emit(Event(tick_name));
            }            
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    // Second Stop
    class SecondStop : public State, public AgentInterface {
        public:
        void entry(const Event& e) { rate = rand() % 2 == 0 ? 2 : -2; }
        void during() { 
            double tolerance = 3.0;
            double xcoor = -250;
            double ycoor = 250;
            move_toward(xcoor, ycoor);

            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the second stop!";
                emit(Event(tick_name));
            }
        }
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    // Third Stop
    class ThirdStop : public State, public AgentInterface {
        public:
        void entry(const Event& e) { rate = rand() % 2 == 0 ? 2 : -2; }
        void during() { 
            double tolerance = 3.0;
            double xcoor = 0;
            double ycoor = 0;
            move_toward(xcoor, ycoor);

            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the third stop!";
                emit(Event(tick_name));
            }
        }
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    class LeaderController : public StateMachine, public AgentInterface {

        public:
        LeaderController() : StateMachine() {

            set_initial(first_stop);
            tick_name = "tick_" + std::to_string(rand()%1000); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class
            add_transition(tick_name, first_stop, second_stop);
            add_transition(tick_name, second_stop, third_stop);
            add_transition(tick_name, third_stop, first_stop);
            first_stop.set_tick_name(tick_name);
            second_stop.set_tick_name(tick_name);
            third_stop.set_tick_name(tick_name);

        }

        FirstStop first_stop;
        SecondStop second_stop;
        ThirdStop third_stop;
        std::string tick_name;

    };

    class Leader : public Agent {

        public:
        Leader(json spec, World& world) : Agent(spec, world) {
            add_process(wc);
        }

        LeaderController wc;

    };

    DECLARE_INTERFACE(Leader);

}

#endif
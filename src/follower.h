#ifndef __FOLLOWER_AGENT__H
#define __FOLLOWER_AGENT__H 

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
            //std::cout << sensor_value(0);
            //move_toward(xcoor, ycoor);

            if (sensor_value(0) < 60) {
                emit(Event("first_brakes"));
            }
            else {
                move_toward(xcoor, ycoor);
            }
            // Move to next
            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reach the first stop!";
                emit(Event("go_second"));
            } 
        
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    // First Brake
    class FirstBrakes : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}
        void during() { 
            track_velocity(0, 0);
            if (sensor_value(0) > 80) {
                emit(Event("first_go"));
            }      
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    // Second Brake
    class SecondBrakes : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}
        void during() { 
            track_velocity(0, 0);
            if (sensor_value(0) > 80) {
                emit(Event("second_go"));
            }      
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    // Third Brake
    class ThirdBrakes : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}
        void during() { 
            track_velocity(0, 0);
            if (sensor_value(0) > 80) {
                emit(Event("third_go"));
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

            if (sensor_value(0) < 60) {
                emit(Event("second_brakes"));
            }
            else {
                move_toward(xcoor, ycoor);
            }

            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the second stop!";
                emit(Event("go_third"));
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
            //std::cout << sensor_value(0);

            if (sensor_value(0) < 60) {
                emit(Event("second_brakes"));
            }
            else {
                move_toward(xcoor, ycoor);
            }

            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the third stop!";
                emit(Event("go_first"));
            }
        }
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    class FollowerController : public StateMachine, public AgentInterface {

        public:
        FollowerController() : StateMachine() {

            set_initial(first_stop);
            tick_name = "tick_" + std::to_string(rand()%999); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class

            add_transition("go_second", first_stop, second_stop);
            add_transition("first_brakes", first_stop, first_brakes);
            add_transition("first_go", first_brakes, first_stop);

            add_transition("go_third", second_stop, third_stop);
            add_transition("second_brakes", second_stop, second_brakes);
            add_transition("second_go", second_brakes, second_stop);

            add_transition("go_first", third_stop, first_stop);
            add_transition("third_brakes", third_stop, third_brakes);
            add_transition("third_go", third_brakes, third_stop);

            first_stop.set_tick_name("go_second");
            first_stop.set_tick_name("first_brakes");
            first_stop.set_tick_name("first_go");

            second_stop.set_tick_name("go_third");
            second_stop.set_tick_name("second_brakes");
            second_stop.set_tick_name("second_go");

            third_stop.set_tick_name("go_first");
            third_stop.set_tick_name("third_brakes");
            third_stop.set_tick_name("third_go");

        }

        // Class to head to the next stop
        FirstStop first_stop;
        SecondStop second_stop;
        ThirdStop third_stop;

        // Class to slow down when too close to the one in front
        FirstBrakes first_brakes;
        SecondBrakes second_brakes;
        ThirdBrakes third_brakes;

        std::string tick_name;

    };

    class Follower : public Agent {

        public:
        Follower(json spec, World& world) : Agent(spec, world) {
            add_process(fw);
        }

        FollowerController fw;

    };

    DECLARE_INTERFACE(Follower);

}

#endif
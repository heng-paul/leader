#ifndef __FOLLOWER2_AGENT__H
#define __FOLLOWER2_AGENT__H 

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
                emit(Event("first_brakes2"));
            }
            else {
                move_toward(xcoor, ycoor);
            }
            // Move to next
            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reach the first stop!";
                emit(Event("go_second2"));
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
                emit(Event("first_go2"));
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
                emit(Event("second_go2"));
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
                emit(Event("third_go2"));
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
                emit(Event("second_brakes2"));
            }
            else {
                move_toward(xcoor, ycoor);
            }

            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the second stop!";
                emit(Event("go_third2"));
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
                emit(Event("second_brakes2"));
            }
            else {
                move_toward(xcoor, ycoor);
            }

            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the third stop!";
                emit(Event("go_first2"));
            }
        }
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    class FollowerController2 : public StateMachine, public AgentInterface {

        public:
        FollowerController2() : StateMachine() {

            set_initial(first_stop2);
            tick_name = "tick_" + std::to_string(rand()%999); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class

            add_transition("go_second2", first_stop2, second_stop2);
            add_transition("first_brakes2", first_stop2, first_brakes2);
            add_transition("first_go2", first_brakes2, first_stop2);

            add_transition("go_third2", second_stop2, third_stop2);
            add_transition("second_brakes2", second_stop2, second_brakes2);
            add_transition("second_go2", second_brakes2, second_stop2);

            add_transition("go_first2", third_stop2, first_stop2);
            add_transition("third_brakes2", third_stop2, third_brakes2);
            add_transition("third_go2", third_brakes2, third_stop2);

            first_stop2.set_tick_name("go_second2");
            first_stop2.set_tick_name("first_brakes2");
            first_stop2.set_tick_name("first_go2");

            second_stop2.set_tick_name("go_third2");
            second_stop2.set_tick_name("second_brakes2");
            second_stop2.set_tick_name("second_go2");

            third_stop2.set_tick_name("go_first2");
            third_stop2.set_tick_name("third_brakes2");
            third_stop2.set_tick_name("third_go2");

        }

        // Class to head to the next stop
        FirstStop first_stop2;
        SecondStop second_stop2;
        ThirdStop third_stop2;

        // Class to slow down when too close to the one in front
        FirstBrakes first_brakes2;
        SecondBrakes second_brakes2;
        ThirdBrakes third_brakes2;

        std::string tick_name;

    };

    class Follower2 : public Agent {

        public:
        Follower2(json spec, World& world) : Agent(spec, world) {
            add_process(fw2);
        }

        FollowerController2 fw2;

    };

    DECLARE_INTERFACE(Follower2);

}

#endif
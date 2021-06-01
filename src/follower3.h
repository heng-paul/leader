#ifndef __FOLLOWER3_AGENT__H
#define __FOLLOWER3_AGENT__H 

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
            // Tolerance of how close we need to get
            // Coordinates for the positon
            double tolerance = 3.0;
            double xcoor = 250;
            double ycoor = 250;

            // Brakes if we get too close to the block in front
            // or just keep going to the stop.
            // Once we get there move to the next stop.
            if (sensor_value(0) < 60) {
                emit(Event("first_brakes3"));
            }
            else {
                move_toward(xcoor, ycoor);
            }
            // Move to next
            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reach the first stop!";
                emit(Event("go_second3"));
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
            // Tolerance of how close we need to get
            // Coordinates for the positon
            double tolerance = 3.0;
            double xcoor = -250;
            double ycoor = 250;
            
            // Brakes if we get too close to the block in front
            // or just keep going to the stop.
            // Once we get there move to the next stop.
            if (sensor_value(0) < 60) {
                emit(Event("second_brakes3"));
            }
            else {
                move_toward(xcoor, ycoor);
            }   

            // Move to next
            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the second stop!";
                emit(Event("go_third3"));
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
            // Tolerance of how close we need to get
            // Coordinates for the positon
            double tolerance = 3.0;
            double xcoor = 0;
            double ycoor = 0;

            // Brakes if we get too close to the block in front
            // or just keep going to the stop.
            // Once we get there move to the next stop.
            if (sensor_value(0) < 60) {
                emit(Event("second_brakes3"));
            }
            else {
                move_toward(xcoor, ycoor);
            }

            // Move to next
            if ( (position().x - xcoor) < tolerance && (position().x - xcoor) > - tolerance && (position().y - ycoor) < tolerance && (position().y - ycoor) > - tolerance)  {
                std::cout << "Reached the third stop!";
                emit(Event("go_first3"));
            }
        }
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    // First Brake
    class FirstBrakes : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}
        void during() { 
            // Start to brake
            track_velocity(0, 0);

            // Start up again once there is space in front
            if (sensor_value(0) > 80) {
                emit(Event("first_go3"));
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
            // Start to brake
            track_velocity(0, 0);

            // Start up again once there is space in front
            if (sensor_value(0) > 80) {
                emit(Event("second_go3"));
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
            // Start to brake
            track_velocity(0, 0);

            // Start up again once there is space in front
            if (sensor_value(0) > 80) {
                emit(Event("third_go3"));
            }      
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    class FollowerController3 : public StateMachine, public AgentInterface {

        public:
        FollowerController3() : StateMachine() {

            set_initial(first_stop3);
            tick_name = "tick_" + std::to_string(rand()%999); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class

            // Add all possible transitions and tick names
            add_transition("go_second3", first_stop3, second_stop3);
            add_transition("first_brakes3", first_stop3, first_brakes3);
            add_transition("first_go3", first_brakes3, first_stop3);

            add_transition("go_third3", second_stop3, third_stop3);
            add_transition("second_brakes3", second_stop3, second_brakes3);
            add_transition("second_go3", second_brakes3, second_stop3);

            add_transition("go_first3", third_stop3, first_stop3);
            add_transition("third_brakes3", third_stop3, third_brakes3);
            add_transition("third_go3", third_brakes3, third_stop3);

            first_stop3.set_tick_name("go_second3");
            first_stop3.set_tick_name("first_brakes3");
            first_stop3.set_tick_name("first_go3");

            second_stop3.set_tick_name("go_third3");
            second_stop3.set_tick_name("second_brakes3");
            second_stop3.set_tick_name("second_go3");

            third_stop3.set_tick_name("go_first3");
            third_stop3.set_tick_name("third_brakes3");
            third_stop3.set_tick_name("third_go3");

        }

        // Class to head to the next stop
        FirstStop first_stop3;
        SecondStop second_stop3;
        ThirdStop third_stop3;

        // Class to slow down when too close to the one in front
        FirstBrakes first_brakes3;
        SecondBrakes second_brakes3;
        ThirdBrakes third_brakes3;

        std::string tick_name;

    };

    class Follower3 : public Agent {

        public:
        Follower3(json spec, World& world) : Agent(spec, world) {
            add_process(fw3);
        }

        FollowerController3 fw3;

    };

    DECLARE_INTERFACE(Follower3);

}

#endif
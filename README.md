# Leader
Final Project for UW EEP 520 Spring 2021

# Overview
- This is a simulation of a leader (a red block) that goes in a loop to three different stops. Followers (blue blocks) will follow in his footsteps but won't get too close. When they do they will stop until there is space in front of them. They know when to stop because they have distance sensors on them.

# Description of Key Challenges
- Things that gave me issues while troubleshooting
  -  The Y coordinate system is upside down which causes confusion.
  -  Running the simulation causes my computer to heat up and slows everything down! This slows down development a lot!
  -  Creating multiple instances of classes tied there states together. Meaning when one stopped all stopped. I worked around this by creating different classes for each follower.

# How to Install and Run the Code
Run the following after cloning the repo
```
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:alpha bash
esm start

make
enviro
```
Then navigate to `http://localhost` to see the simulation.

You can press `Ctrl-C` to stop the enviro server.

# Acknowledges of Sources of Infomation or Code used
Code is an alteration of the following:
`https://github.com/tbonaciUW/EEP_520_Spring2021/tree/main/week9/wanderers`

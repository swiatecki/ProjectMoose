# Worklog

### Design of model/controller
* Detect delay + integrator (model01.m)
* Test simple step on model01 and compare with real step(trackDelayNetwork/matlab.m - in a section.) These match. 
* Design a initial controller via 60 degrees phasemargin (Model01/Controller01/controller01.m). Kp = 18.20 (25.2 dB) at 120 rads.
* Compare step response of closed loop controller01 with actual output. (SE next step)
* Robot has a max velocity, empirically found = 3.2 rad/s. We need a saturation. NEW MODEL - this time in simulink)
* ~~Model01_limited.mdl~~ Model02.mdl comes to life. This has a saturation 
* Model02 not good enough, implement rate-limit, to simulate acc. 
* Determine which stepsizes should be used! (physical calculation)
* Simulate in matlab(contSim02.m) + reallife(contImp02.cpp))




# TODO


* Take screenshots of Model02 sim w/o rate-limit
* Take screenshots of Model02 sim w. rate-limit


## COVID-19 Simulation Model

This document describes the discrete, agent-based SAIVR model we have implemented for our COVID-19 simulation. The first section describes the fundamental model required for the simulation to do something useful, while the second section describes different extensions that could be made to the model. This document does not describe the generation of the graphs used for the simulation.


## Current Model
The model we will implement is an extension of the SIR model described by Kaxiras and Neofotistos[1]. Our model adds two additional compartments: asymptomatic and vaccinated, and moves from a matrix approach to a graph-based approach. This section describes how this model works in order to achieve its minimal goals.


### Model Goals
Our model was to be able to describe how the disease spreads through a population, and should also be able to be used for investigating different vaccination deployment strategies. The basic strategies that could be tried would involve deploying to highly infected areas first, deploying to areas with few infections first, or deploying randomly. We would also like to model asymptomatic behaviour, and according to Kronbichler et al.[2], “among other factors, age has been described as a risk factor for a more severe disease course, while younger people seem to have mild or even asymptomatic presentations and thus might be crucial in further spreading of the disease”.


### Model Overview
Based on the goals for the model, we needed to represent the five different compartments of SAIVR, regions of populations, and age groups. The agents are connected in an undirected graph, where the edges represent any type of contact between two agents that could transmit the disease. Agents in the same region will have connections between each other, and infection across regions is done through a different mechanic.


#### Compartments
An agent is at any time within one of the compartments **S**usceptible, **A**symptomatic, **I**nfected, **V**accinated or **R**ecovered, and behaves differently depending on which one they belong to.


*  Susceptible agents represent people who have not been infected yet, and are able to become asymptomatic, infected or vaccinated.
*  Asymptomatic agents can infect susceptible agents much like infected agents can, become recovered after some time, but are counted separately, and can have their parameters set separately.
*  Infected agents can transmit the infection to susceptible agents and become recovered after some time.
*  Vaccinated agents can neither spread the disease nor be infected.
*  Recovered agents can neither spread the disease nor be infected.


![alt_text](1-saivr.png "The SAIVR compartments.")


#### Interaction between two agents

The simulation is divided into time steps. In each time step, each infected or asymptomatic agent will attempt to infect either (1) all its neighbours, or (2) a random subset of its neighbours. Each infection attempt is done by calculating the probability of infection between the two agents, sampling from a uniform distribution between 0 and 1, and seeing if this sampled value is lower than the probability. If it is, the susceptible agent is infected. The agent attempting to infect has a parameter for infectiousness. A parameter can also be used for the susceptibility of the target, which can be multiplied with the infectiousness to get the infection probability. The susceptibility parameter can be set to 1 if all agents should be fully susceptible.

The image below shows an example of how the probability of infection p is calculated when agent 1, with infectiousness p<sub>i</sub> tries to infect its neighbour agent 2, with susceptibility s.


![alt_text](2-p=pi_x_s.png "The probability of infection p = p_i * s.")


Upon being infected, the susceptible agent has some probability of instead becoming asymptomatically infected. This is sampled the same way as the infection itself, except with a different probability.


#### Selecting infection targets
As mentioned, we have two methods for selecting neighbours to infect. The first method simply tries to infect all neighbouring agents. The second method selects only a subset of neighbours, by sampling with replacement. In the original model, each agent only selected a single random neighbour each time step. We have added a parameter for this, such that the agent will select n random neighbours to try to infect instead.


#### Age groups
Each agent belongs to an age group, which lets us model things like e.g. having different rates of asymptomatic infections. Each age group can have different parameters, but some parameters can be left the same if we want them to be shared. The specific ages do not have to be strictly defined, and the number of age groups and their parameters can be defined at runtime. This representation is general enough that we could use it for groups other than age, e.g. people with weak immune systems. The image below shows how each agent has a reference to one age group from which they will get their parameters.


![alt_text](3-age-groups.png "Age groups and their parameters.")


One set of age groups that could be used could be the ones used by Guan et. al.[3], i.e. 0-14, 15-49, 50-64 and >=65. The current implementation assigns age groups randomly among all agents, with equal probability for all groups.


#### Regions
Each agent will belong to a specific region, which can represent things like countries, municipalities, or towns. Agents in different communities are not connected directly to each other. However, infected agents can travel to neighbouring regions. At each time step, infected and asymptomatic agents sample a Bernoulli distribution to see whether or not they will travel. If they do not travel, they simply try to infect neighbours in their own region as previously described. If they do travel, a random connected region will be selected, and the agent will try to infect random targets in that region.


#### Vaccination
At a certain time step in the simulation, vaccinations will begin to deploy to susceptible agents. After a certain amount of time, susceptible agents that have been vaccinated will enter the Vaccinated compartment with a given probability. If they do not succeed in being vaccinated, they stay susceptible. If susceptible agents are infected before they succeed in vaccination, vaccination will no longer be possible.

Only a certain amount of vaccines will be available each day, to represent the fact that production takes time. Vaccine deployment can follow different strategies. The currently implemented strategies are a random strategy, where every agent has the same probability of being vaccinated, the low density strategy, where agents with few infected neighbours are infected first, and the high density strategy, where agents with many infected neighbours are vaccinated first.


#### Parameters
General parameters:

Selection type - A value that simply tells us whether we should use method (1) or method (2) when selecting neighbours to try to infect.

Vaccination strategy - Which strategy should be used for deploying the vaccine.

T (simulation duration) - The total number of time steps in the simulation

initial_infections (initial infected count) - The number of infected agents at the start of the simulation

T<sub>v</sub> (vaccination start time) - The time at which the vaccine starts being deployed.

n<sub>v</sub> (vaccine count) - The number of vaccines available each time step.

Parameters for each age group, some could be set to be the same for different age groups. Some parameters, like d<sub>i</sub> and d<sub>ai</sub> could reasonably be set to be the same, but would be simple to implement separately in case it ever becomes desirable to have them be different:

n<sub>i</sub> (infection attempts) - The number of random neighbours an infected agent should try to infect each time step, when method (2) is used.

n<sub>ai</sub> (asymptotic infection attempts) - The number of random neighbours an asymptotic agent should try to infect each step, when method (2) is used.

susceptibility - A value between 0 and 1, multiplied with an infection probability to lessen the probability of being infected.

p<sub>i</sub> (infectiousness) - A value between 0 and 1, used as a parameter to a Bernoulli distribution that determines whether the agent infects a susceptible agent or not. This is multiplied by the target’s susceptibility.

p<sub>ai</sub> (asymptomatic infectiousness) - Used the same way as infectiousness, but is instead used when the infecting agent is asymptomatic.

p<sub>t</sub> (travel probability) - The probability for an agent to travel in a given time step.

p<sub>at</sub> (asymptomatic travel probability) - The probability of travelling if the agent is asymptomatic.

p<sub>v</sub> (vaccine effectiveness) - A value between 0 and 1, used for a Bernoulli distribution to sample whether or not a vaccination succeeds.

d<sub>v</sub> (vaccine duration) - The time taken for vaccination to become effective after being vaccinated.

d<sub>i</sub> (infection duration) - The time taken for an agent to go from infected to recovered.

d<sub>ai</sub> (asymptotic infection duration) - The time taken for an agent to go from asymptomatic to recovered.

a<sub>p</sub> (asymptomatic chance) - The probability of becoming asymptomatic when being infected.


## Potential extensions
This section describes some extensions that could be made to the model after the fundamental model is completed. They would have to be fleshed out a bit more before implementation.


### Additional vaccination methods
We might want to make it possible to investigate additional vaccination methods. Some methods that we could try would be to base the vaccination deployment on age groups instead of regions. Another would be to vaccinate in an annulus around a region with a lot of infections, which seems to be something Tildesley et al. investigated for livestock vaccination.


### Isolation
We could simulate the isolation of infected agents in different ways. Asymptotic agents should not be isolated, since they do not know that they can infect people. The simplest way for selection method (2) could be implemented with the fundamental model by simply having n<sub>i</sub> be lower than n<sub>ai</sub>. Another method would be to limit the number of people who can be infected by an agent to a smaller set of its neighbours. This subset of neighbours would be the same for each time-step, which would mimic D shrinking in the matrix model.


### Lockdowns
We could do something similar to what the matrix model did with a shrinking value for D after a certain point in time. This implementation can be done similarly to isolation, except with a parameter that changes based on the current time step.


### Different types of edges
We could try to integrate something similar to the edges module written earlier in the project in order to represent different types of contact between two agents. This would however probably add a lot of complexity to the graph generation.


# References
[1] Kaxiras, Efthimios, and Georgios Neofotistos. "Multiple epidemic wave model of the covid-19 pandemic: Modeling study." *Journal of Medical Internet Research* 22.7 (2020): e20912.

[2] Kronbichler, Andreas, et al. "Asymptomatic patients as a source of COVID-19 infections: A systematic review and meta-analysis." *International journal of infectious diseases* 98 (2020): 180-186.

[3] Guan, Wei-jie, et al. "Clinical characteristics of coronavirus disease 2019 in China." *New England journal of medicine* 382.18 (2020): 1708-1720.

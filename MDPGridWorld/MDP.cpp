//
//  MDP.cpp
//  AI_GridWorld
//
//  Created by cpsfaculty on 11/13/17.
//  Copyright (c) 2017 ___Ju Shen___. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "MDP.h"


/*************************************************************** Below are the two functions you need to implement ****************************************************/


/*Compute a Q-value for a given state and its action
  Input: state variable s; action (go to East/North/West/South), the corresponding interger value for each action is defined in the "MDP.h" Macros definition on the top
  Output: you need to update the corresponding q_values of the input state variables
 */

void MDP::computeQValue(State &s, const int action)
{
//    std::cout << s.location.x << s.location.y << std::endl;
//    std::cout << action << std::endl;
//    std::cout << "****" << std::endl;
    
    float q0 = 0;
    float q1 = 0;
    float q2 = 0;
    float q3 = 0;
    
// Function to calculate probability of Landing East
    auto east = [&](float transition)
    {
        if (s.location.x == 2 && s.location.y == 0){
            q0 = transition * (ACTION_REWARD + GAMMA * 1);

        } else if (s.location.x == 2 && s.location.y == 1){
            q0 = transition * (ACTION_REWARD + GAMMA * (-1));

        } else if ((s.location.x == 0 && s.location.y == 1) || (s.location.x == 3 && s.location.y == 2)){
            q0 = transition * (ACTION_REWARD + GAMMA * s.state_value);

        } else {
            q0 = transition * (ACTION_REWARD + GAMMA * states[s.location.y][s.location.x+1].state_value);
        }
        return q0;
    };
    
// Function to calculate probability of Landing South
    auto south = [&](float transition)
    {
        if (s.location.y == 2 || (s.location.x == 1 && s.location.y == 0)){
            q1 = transition * (ACTION_REWARD + GAMMA * s.state_value);

        } else {
            q1 = transition * (ACTION_REWARD + GAMMA * states[s.location.y+1][s.location.x].state_value);
        }
        return q1;
    };
    
// Function to calculate probability of Landing West
    auto west = [&](float transition)
    {
        if (s.location.x == 0 || (s.location.x == 2 && s.location.y == 1)){
            q2 = transition * (ACTION_REWARD + GAMMA * s.state_value);

        } else {
            q2 = transition * (ACTION_REWARD + GAMMA * states[s.location.y][s.location.x-1].state_value);
        }
        return q2;
    };
    
// Function to calculate probability of Landing North
    auto north = [&](float transition)
    {
        if (s.location.y == 0 || (s.location.x == 1 && s.location.y == 2)){
            q3 = transition * (ACTION_REWARD + GAMMA * s.state_value);

        } else if (s.location.x == 3 && s.location.y ==2){
            q3 = transition * (ACTION_REWARD + GAMMA * (-1));

        } else {
            q3 = transition * (ACTION_REWARD + GAMMA * states[s.location.y-1][s.location.x].state_value);
        }
        return q3;
    };

    
// Root conditions to skip q_values for wall_state:states[1][1], diamond_state:states[0][3], and pitfall_state:states[1][3]
    
    if ((s.location.x != 1 && s.location.y != 1)||(s.location.x != 3 && s.location.y != 0) || (s.location.x != 3 && s.location.y != 1))
    {

        if (action == ACTION_EAST)
        {
            q0 = east(TRANSITION_SUCCEED);
            q1 = south(TRANSITION_FAIL/2);
            q3 = north(TRANSITION_FAIL/2);
            q2 = 0;
            if (cur_convergence < abs(s.q_values[ACTION_EAST] - (q0 + q1 + q2 + q3)))
                cur_convergence = abs(s.q_values[ACTION_EAST] - (q0 + q1 + q2 + q3));
            s.q_values[ACTION_EAST] = q0 + q1 + q2 + q3;
        }
        
        else if (action == ACTION_SOUTH)
        {
            q1 = south(TRANSITION_SUCCEED);
            q0 = east(TRANSITION_FAIL/2);
            q2 = west(TRANSITION_FAIL/2);
            q3 = 0;
            if (cur_convergence < abs(s.q_values[ACTION_SOUTH] - (q0 + q1 + q2 + q3)))
                cur_convergence = abs(s.q_values[ACTION_SOUTH] - (q0 + q1 + q2 + q3));
            s.q_values[ACTION_SOUTH] = q0 + q1 + q2 + q3;
        }
        
        else if (action == ACTION_WEST)
        {
            q2 = west(TRANSITION_SUCCEED);
            q1 = south(TRANSITION_FAIL/2);
            q3 = north(TRANSITION_FAIL/2);
            q0 = 0;
            if (cur_convergence < abs(s.q_values[ACTION_WEST] - (q0 + q1 + q2 + q3)))
                cur_convergence = abs(s.q_values[ACTION_WEST] - (q0 + q1 + q2 + q3));
            s.q_values[ACTION_WEST] = q0 + q1 + q2 + q3;
        }

        else if (action==ACTION_NORTH)
        {
            q3 = north(TRANSITION_SUCCEED);
            q0 = east(TRANSITION_FAIL/2);
            q2 = west(TRANSITION_FAIL/2);
            q1 = 0;
            if (cur_convergence < abs(s.q_values[ACTION_NORTH] - (q0 + q1 + q2 + q3)))
                cur_convergence = abs(s.q_values[ACTION_NORTH] - (q0 + q1 + q2 + q3));
            s.q_values[ACTION_NORTH] = q0 + q1 + q2 + q3;
        }
        
    } // End of loop computing q_values for next action
}



/*There is no return value, actually you need to use the computing result to update the state values of all the states defined as data member "State states[3][4]". Of course, you don't need to update the wall state: states[1][1], the diamond state: states[0][3], and pitfall state: states[1][3] */
void MDP::valueIteration()
{
    cur_convergence = 0;
    
    // Computing q_values
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<4; j++)
        {
            computeQValue(states[i][j],ACTION_EAST);
            computeQValue(states[i][j],ACTION_SOUTH);
            computeQValue(states[i][j],ACTION_WEST);
            computeQValue(states[i][j],ACTION_NORTH);
        }
    }
    
    // Updating q_values
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<4; j++)
        {
            if ((i == 0 && j == 3) || (i == 1 && j == 3) || (i == 1 && j == 1)){
                continue;
            } else {
                float maxq = *std::max_element(states[i][j].q_values, states[i][j].q_values+4);
                states[i][j].state_value = maxq;
            }
        }
    }

}


/**********************************************************************    End of Assignment *********************************************************************/


MDP::MDP()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
        }
    }
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    
    /*Set the current convergence to a big number initially*/
    cur_convergence = 100; //the reason this value is set to a big value is to ensure
    
    
}


MDP::~MDP()
{
    
}

/*Reset the current computed state and Q values*/
void MDP::resetData()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
        }
    }
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    
    /*Set the current convergence to a big number initially*/
    cur_convergence = 100; //the reason this value is set to a big value is to ensure
    
    
}


#pragma once

#include <unordered_map>
 using std::unordered_map;
#include <string>
 using std::string;
#include <functional>
 using std::function;
#include <vector>
 using std::vector;
#include <iostream>
 using std::cout;
 using std::endl;

struct Fsm;

using Condition=function<bool(Fsm&)>;
using Action=function<void(Fsm&)>;
 
struct Transition
{
 string current_state;
 Condition condition;
 Action action;
 string next_state;
};

struct Step
{
 size_t i_step;
 size_t i_str;
 string state_from;
 string state_to;
 string buffer_value;
};

struct Fsm
{
 bool detailed=false;
 string str;
 size_t i_str;
 size_t i_step;
 string current_state;
 string buffer;
 vector<Transition> transitions;
 vector<Step> steps;
 Fsm(string s):str(s),i_str(0),i_step(0),current_state("Initial"){}
 void add_transition(const string& current_state,
  Condition condition,
  Action action,
  const string& next_state)
 {
  transitions.push_back({current_state,condition,action,next_state});
 }
 void run()
 {
  while(current_state!="Idle"&&current_state!="Final")
  {
   bool transition_performed=false;
   for(const auto& transition:transitions)
   {
    if(i_str<str.size())
    {
     if(transition.current_state==current_state && transition.condition(*this))
     {
      if(detailed)
       cout<<"Current state: "<<current_state<<endl;
      size_t log_i_step;
      size_t log_i_str;
      string log_state_from;
      string log_state_to;
      string log_buffer;
      // Set log fields - before action.
      log_i_step=i_step;
      log_i_str=i_str;
      log_state_from=current_state;
      // Action and next state.
      transition.action(*this);
      current_state=transition.next_state;
      // Set log fields - after action.
      log_state_to=current_state;
      log_buffer=buffer;
      // Store log item.
      steps.push_back({log_i_step,log_i_str,log_state_from,log_state_to,log_buffer});
      // Continue;
      i_step++;
      transition_performed=true;
      break;
     }
    }
    else
    {
     if(detailed)
      cout<<"Stopping - end of string reached"<<endl;
     current_state="Final";
     break;
    }
   }
   if(!transition_performed)
   {
    if(detailed)
     cout<<"Stopping - idle"<<endl;
    current_state="Idle";
    break;
   }
  }
 }
};

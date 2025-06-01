#include "Fsm.hpp"

// Conditions.
bool is_start(Fsm& fsm) { return true; }
bool is_rightarrow(Fsm& fsm) { return fsm.str[fsm.i_str]=='>'; }
bool is_validchar(Fsm& fsm) { return (fsm.str[fsm.i_str]!=' ')&&(fsm.str[fsm.i_str]!='>'); }
bool is_space(Fsm& fsm) { return fsm.str[fsm.i_str]==' '; }
bool is_nomorechars(Fsm& fsm) { return fsm.i_str>=fsm.str.length(); }

// Actions.
void next_char(Fsm& fsm) { fsm.i_str++; }
void readto_buffer(Fsm& fsm) { fsm.buffer+=fsm.str[fsm.i_str]; fsm.i_str++; }
void delete_buffer(Fsm& fsm) { fsm.buffer=""; fsm.i_str++; }
void no_action(Fsm& fsm) { }

int main (int argc, char* argv [])
{

 //Fsm fsm("       R[RYB]            R[RBW]            R[RGY]            R[RWG]            O[OBY]            R[OWB]            O[OYG]            O[OGW]      ");
 //Fsm fsm("       R[RYB]");
 Fsm fsm("     RYB>R          RBW>R          RGY>R          RWG>R          OBY>O          OWB>O          OYG>O          OGW>O     ");

 fsm.add_transition("Initial",         is_start,      next_char,     "BeforeFaces");
 fsm.add_transition("BeforeFaces",     is_space,      next_char,     "BeforeFaces");
 fsm.add_transition("BeforeFaces",     is_validchar,  readto_buffer, "StartFaces");
 fsm.add_transition("StartFaces",      is_validchar,  no_action,     "InFaces");
 fsm.add_transition("InFaces",         is_validchar,  readto_buffer, "InFaces");
 fsm.add_transition("InFaces",         is_rightarrow, no_action,     "AfterFaces");
 fsm.add_transition("AfterFaces",      is_rightarrow, delete_buffer, "BeforeDirection");
 fsm.add_transition("BeforeDirection", is_validchar,  readto_buffer, "InDirection");
 fsm.add_transition("InDirecton",      is_validchar,  readto_buffer, "InDirection");
 fsm.add_transition("InDirection",     is_space,      no_action,     "AfterDirection");
 fsm.add_transition("AfterDirection",  is_space,      delete_buffer, "BeforeFaces");
 fsm.add_transition("AfterDirection",  is_space,      next_char,     "BeforeFaces");

 fsm.detailed=true;
 fsm.run();

 for(size_t i_step=0;i_step<fsm.steps.size();i_step++)
 {
  cout<<
   "before action:"<<
   " i_step="<<fsm.steps[i_step].i_step<<
   " i_str="<<fsm.steps[i_step].i_str<<
   " state="<<fsm.steps[i_step].state_from<<
   " char='"<<fsm.str[fsm.steps[i_step].i_str]<<"'"<<
   " after action:"<<
   " state="<<fsm.steps[i_step].state_to<<
   " buffer="<<fsm.steps[i_step].buffer_value<<
  endl;
  string state_reached=fsm.steps[i_step].state_to;
  if(state_reached=="AfterDirection")
  {
   string direction=fsm.steps[i_step].buffer_value;
   cout<<" Direction: "<<direction<<endl;
   continue;
  }
  if(state_reached=="AfterFaces")
  {
   string faces=fsm.steps[i_step].buffer_value;
   cout<<" Faces: "<<faces<<endl;
  }
 }

 return 0;

}


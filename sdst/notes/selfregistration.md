Self Registration
=======

```xml
<unit>
   <tasks>
      <task type="attack" />
      <task type="gather" />
      <task type="move" />
   </tasks>
</unit>
```

Loading code
```cpp
// EntityDefinition std::vector<Task*> m_taskPrototypes; 

bool EntityDefinition::LoadFromXML( XMLNode node ) 
{
   XMLNode tasks = node.get_first_child("tasks"); 
   if (tasks != nullptr) {
      XMLNode task = tasks.get_first_child("task");
      while (task != nullptr) {
         std::string type = task.get_attribute("type"); 

         // this is what I have
         // go find a callback for creating name
         if (type == AttackTask::NAME) {
            new_task = new AttackTask(); 
         } else if (type == "move") {
            new_task = new MoveTask(); 
         } else {
            //...
         } 

         ASSERT( new_task != nullptr ); 
         if (new_task != nullptr) {
            new_task->LoadFromXML( task ); 
            m_taskPrototypes.push_back( new_task ); 
         }

         task++; 
      }
   }
}

class Task 
{
   ...
};

class MoveTask : public Task
{

};

class AttackTask : public : Task 
{

};
```

What I want to get to
```cpp
   Task *new_task = Task::CreateForName(type); 
   if (new_task != nullptr) {
      new_task->LoadFromXML( task ); 
      m_taskPrototypes.push_back( new_task ); 
   }

//
STATIC Task* Task::CreateForName( std::string const& name ) 
{
   task_create_cb create_cb = TaskRegistrator::FindForName( name ); .
   if (create_cb) {
      return creabe_cb(); 
   } else {
      ASSERT(0); // let the user know something messed; 
      return nullptr; 
   }
}

```


What happens first when a program runs?
```cpp

static int gIntValue = 0; 
static std::vector<int> gIntList; 

class Singleton
{
   public:
      static Singleton *s_pointer = nullptr; 
      static Singleton s_instance; 

      static Singleton* GetInstance() 
      {
         if (s_pointer == nullptr) {
            s_pointer = new Singleton(); 
         }

         return s_pointer; 
      }
};

class OhHey
{
   public:
      OhHey() 
      {
          Singleton::s_instance.DoThing(); 
          Singleton::GetInstance()->DoThing();  
         ASSERRT( gIntValue == 0 ); printf("OhHiThere\n"); 
      }
};

static OhHey gHey; 


class TaskRegistrar
{
   public:
      TaskRegistrar( char const *name, task_create_cb cb )
      {
         m_name = name;
         m_create_cb = cb; 

         s_createLookup[name] = cb; 
      }

   public:
      std::string m_name; 
      task_create_cb m_create_cb; 

   public:
      static std::map<std::string, m_create_cb> s_createLoookup; 

      static m_create_cb FindForName( std::string const &name )
      {

      }
}; 

static TaskRegistrator gAttackTaskRegister( "attack", []() { return  new AttackTask(); } ); 

int main()
{
   printf( "Hello World\n" ); 
   return 0; 
}

class AttackTask
{
   private:
      static TaskRegistrator s_register( "attack", [](...) ); 
}

// AttackTask.cpp
static TaskRegistrator gAttackTaskRegister( "attack", []() { return  new AttackTask(); } ); 

DEFINE_TASK( "attack", AttackTask ); 
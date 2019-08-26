#pragma once

#define UNITTEST( name, category, priority )  /*IMPLEMENT ME*/

// optional if you want to use this - I find it makes tests read a little better
#define CONFIRM(e) 	if (!(e)) { return false; }

// Will run all categories, any test that has priority <= the passed in priority
void UnitTestsRunAllCategories( int priority = INT_MAX ); 

// Runs all unit tests that are part of this category (nullptr counts as a valid category). 
void UnitTestsRun( char const* category, int priority = INT_MAX ); 
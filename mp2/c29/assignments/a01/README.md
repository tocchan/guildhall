MP2.A01 Git Setup
======

## Overview
Get your projects setup within GitHub.  Try to have your project compiling by Thursday before lecture as we'll be making the D3D11 branch
used in the SD2 class. 

------

## Checklist
Use the branch name `mp2/turnin/a01` for grading, and you have the checklist copied to your `Root` folder in a file named `mp2.a01.md`

- [ ] [05pts] Create a GitHub account 
- [ ] [05pts] Send me your Username
- [ ] [10pts] Create Private Repots
    - [ ] `Guildhall` repot
    - [ ] `Engine` repot if using submodules
- [ ] [10pts] Make me a collaborator on all depots needed for class
    - [ ] `Guildhall` Repot
    - [ ] `Engine` Repot if using Sbubmodules
- [ ] [20pts] Properly setup [./example.gitignore](`.gitignore`) for all repots
    - [ ] Not intermediate or temporary files should be pushed to the depot
    - [ ] Only the `exe` in the `Run` folder exists. 
- [ ] [05pts] Make initial commit for all repots
- [ ] [05pts] Make initial push for all repots
- [ ] [10pts] Add a `mp2.a01.md` file to your `Root` folder...
    - [ ] This should contain a copy of this checklist with tasks you've attempted marked
    - [ ] Make sure this is pushed
    -
- [ ] [30pts] Create your `mp2/turnin/a01` branch and make sure it is pushed to remote.
- [ ] Do a buddy build to make sure you can pull and build it yourself
- [ ] Once assignment is done and everything compiles (and there were changes), merge `mp2/turnin/a01` back in to `master`

------

## Notes

### Buddy Build
To do a buddy build, make a new folder, for example `D:\BuddyBuild`, and make sure it is empty.

Afterwards, do a fresh clone and build and make sure everything runs.  See syllabus for late penalty if I'm unable to build your project.

```
git clone https://github.com/your_account/guildhall.git 
cd guildhall
git submodule init
git submodule update
```

Open the solution, compile, run, and quit the application!  I grade D3D leaks based on output when the device is destroyed,
so if you crash on shutdown, it will be considered max penalty. 


### Getting Ready for A02
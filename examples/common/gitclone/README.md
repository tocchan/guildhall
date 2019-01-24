Mirroring Protogame
======

## Mirror a Repot
From the following tutorial; [Duplicating a repository, help.github.com](https://help.github.com/articles/duplicating-a-repository/)

This will create a copy of your repot as another repot, maintaining submodule links and all history; 

```bash
# Clone the old repot's git information (just the .git folder)
git clone --bare https://github.com/user/repot.git 

# Move to the new folder
cd ./repot.git 

# Push this as a mirror to your newly created repot (should already be created on github.com)
git push --mirror https://github.com/user/new_repot.git 

# Remove the local copy of the old repot we used for this; 
cd .. 
rm -rf ./repot.git 
```

## Clone a Repot
At this point, you do not have a local copy, but github should now be up to date; To clone, including submodules, you would then do;

```bash
# Navigate to our projects folder (where yours is)
D:
cd ./Projects

# Clone our new repot 
git clone https://github.com/user/new_repot.git 

# Navigate to it; 
cd ./new_repot

# Get the submodule(s)
git submodule init 
git submodule update 
```

**IMPORTANT**  Git default behaviour for submodule's is to be "headless", ie, just points to a commit number, but not be a part of any specific branch, so any changes done to the submodule will not be tracked.  So be sure that, if you'll be working in the submodule (which you will be for this class), to navigate to the submodule, and checkout the appropriate branch, for example...

```bash
# Assuming we're still in the correct in our root projects git folder
cd ./Path/To/Submodule

# Get information about the repot, including available remote branches (not always needed, but probably good to do first time)
git fetch -p

# Checkout the appropriate branch, for example, the feature/d3d11 branch our first project will be referencing
git checkout feature/d3d11
```

## Changing Branches with Submodules
Submodules do not automatically change with the branch you are in, so be sure that when you switch branches, to update any submodules you're using;

```bash
# Assume I'm back in the root projects git filder, for example, D:\projects\rts
# This will checkout a branch
git checkout feature/pathfinding

# This will be sure the submodule is at the right version for this branch
# WARNING: if there are changes in the submodule, this may cause you to lose them, or fail out; 
git submodule update 
```

SourceTree seems to automatically do the above step when switching branches, but do be careful that the submodule will be headless, so be sure, if doing work in the submodule that a branch is either checkout or created for the work you're doing. 


## Resource
- [Duplicating a repository, help.github.com](https://help.github.com/articles/duplicating-a-repository/)
- [Shallow Copies, perforce.com](https://www.perforce.com/blog/vcs/git-beyond-basics-using-shallow-clones)

Git Setup
======

## Personal Opinions on Git -vs- Perforce

### Pros
- Easier to collaborate
- Code review functionality built in with pull-requests
- Can easily change gears and do work in parallel with branches. 
  - P4 has streams and shelving, but have always found them a bit more clunky to work with.
- Can work offline
- Your github account is your own, not Guildhall's, meaning you can take it with you when you leave. 
- GitHub links make it easier to share your projects in your portfolio. 
- Makes grading easier!
- Really good to hav experience with both!

### Cons
- Large files (Art!).  Git is great for code, but not for binary assets.  File size limit is fairly small (50MB warning, 100MB limit). 
- No exclusive checkout (Git is local, there is *no checkout*, perioid), so unmergable file formats are risky in teams.
- No visibility into what other users are working with (outside of pushed branches)
- Accidently becoming headless can be very scary the first time it happens - easy to lose work if you don't notice.   





## Current Folder Setup

### Perforce Structure
- `Root/`
  - `Engine/`
    - `Code/`
      - `Engine/`
        - ...
      - `ThirdParty/`
      - ...
  - `Protogame/`
     - `Code/`
       - `Game/`
         - ...
     - `Run`
       - ...
  - *other projects...*

So, trying to maintain this structure, we'll be making our Git repot at the *`Root/`* location.  

In practice, I would recommend having each project be its own repot, and reference shared code (like `Engine` using submodules).  Though for course work we'll just be using one and treating it mostly as a P4 depot.  

### Submodules (Optional)

- **Submodules** are references to another repot within your repot. 
- Useful for Third Party modules (stbi, physx, box2d, etc...) that have git repots, as you can stay linked to the actual repot, but stay synced to a specific point in time that you can choose to move foward at your leisure.

- Not a lot of benefit to the the work we'll be doing in class, and do create more work during submissions
  - Must submit submodules before the parent repot, so the parent repot sees the change.

- More useful if you plan to have some private repots not in your Guildhall folder.  Mainly, this just gives you more options later, or gives you experience with this feaure of Git if you're curious.

Note that you can always change later without too much work, so you're not locked into this decision. 

## Setup

## With Submodules
1. Setup GitHub Account
2. Tell me your Username
3. Make a new local folder on your machine: `D:\Git` for example (name actually doens't matter here)
4. Install SourceTree (or your favorite Git client - if you're comfortable command line you can just stick with thtat as well)

### If Using Submodules...
0. Create private `Guildhall` and `Engine` repots on the GitHub website. 
1. Copy your top level `Engine` folder to your `D:\Git\Engine`
2. Init a git repot here, and make your initial commit
   - Don't forgot to add a `.gitignore` file when you do this; 
3. Link this with your remote depot associated with your Engine, and make your initila push
4. Copy your `Root/` folder, copied to `D:\Git\Guildhall`
5. Delete Engine out of the root folder (we don't want to track it as part of this repot)
6. Init and make your initial commit as a local repot.
7. Tie this to the remote `Guildhall` repot, and make your initial push.
8. Add your `Engine` as a submodule to `Guildhall`, push this change.


### If Not Using Submodules
0. Create a remote private repot called `Guildhall` on the GitHub website. 
1. Copy the contents of `Root/` to `D:/Git/Guildhall`
2. Init your local repot here, and make your initial commit.
   - Don't forget to add a `.gitignore` file when you do this; 
3. Link this to your remote GitHub Guildhall repot, and make your initila push.

### Test
So, Git is local, so we can setup a git repot anywhere on our machine.  This makes 


## Without Submodules

1. Setup GitHub Account
   - Send me your username!
2. Decide on your folder structure
   - If not using submodules, copy your root SD folder somewhere.
   - If you are using submodules, copy your `Protogame` and `Engine` projects to a new location.
3. Create your repot(s) on *GitHub*
   - Just `Guildhall` if not using submodules
   - `Protogame` and `Engine` if doing  


## Buddy Build Check
Doing a buddy build is a bit simpler with Git, so you can test your code the exact same way I will; 

### SourceTree

### Command Line
```
git clone https://github.com/yourusername/yourrepot.git
cd yourrepot
git submodule init
git submodule update 
```

Last two lines only needed if using submodules; 

For example, I will grade by running a script that does effectively...
```
rmdir username
git clone --branch "mp2/a01" https://github.com/username/guildhall.git username
cd username
git submodule init
git submodule update
```

Which pulls the speciifc branch for the assignment turnin to a folder with your username, and then compiling the soluiton I find there;  

## SourceTree


### Pull-Requests
GitHub also have a concept called a **Pull-Request**.  Work in Git is almost always done in a branch, until you're happy with it  
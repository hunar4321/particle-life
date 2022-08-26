![GitHub repo size](https://img.shields.io/github/repo-size/hunar4321/life_code)
![GitHub](https://img.shields.io/github/license/hunar4321/life_code)

# Particle Life Simulation
A simple program to simulate attraction/repulsion forces between many particles, producing life like patterns as a result. The code is less than a page (excluding GUI elements). Video tutorial and Walkthrough is available below.

Learn More Here (YouTube video tutorial):
-----------------------------------------------
https://youtu.be/0Kx4Y9TVMGg


Interface
--------------------------------------------------------
![](images/interface.jpg)

Example Results
--------------------------------------------------------
![](images/big_pic.jpg)

Some Interesting Patterns to Reproduce:
-------------------------------------
You don't need to be exact with the parameters to reproduce these patterns. The best way to get interesting patterns: First try random parameter explorations, once you find an intersting pattern, try fine-tuning it gradually. To avoid being stuck at a local maximum, you can make some occasional big parameter jumps. This way interesting & different patterns keep poping up.

![](images/some_patterns.jpg)

To use:
-------------
Download this repo. unzip the file then go to /particle_life/bin/ folder and click on particle_life.exe

Code:
----------------
Source code available in both C++ and JavaScript ( watch this YouTube video for a walkthrough tutorial: https://youtu.be/0Kx4Y9TVMGg )

If you like to contribute to the C++ program. The core algorithm is the first 100 lines of code at:  "/particle_life/src/ofApp.cpp". Everything else are GUI components & rendering controls which are provided by the openFrameworks library. openFrameworks is a nice, opensource, and easy to use image rendering library. 
To start, download this repository first, then download openFrameworks library here (https://openframeworks.cc/). Use openFramework's projectGenerator and import /particle_life/ folder to the project. Alternatively, generate a new openFramework project and add ofxGui. Once the project files are generated replace the /src/ folder with the one provided here. You can now compile the C++ code on your machine.

Todos (If I have time):
--------------------
1. Adding the ability to save and load parameters (so that people can easily share the interesting models they find)
2. Ability to add more particle types (currently it's fixed to four particle types)
3. Currently, the biggest bottle neck is the nested for-loops (which calculates the pairwise distance among all particles) making the computational complexity quadratic. It will be amazing if we could find a way around this bottle neck.
4. Alternative to point 3, computing the pairwise distances are embarrassingly parallel so it can be computed on GPU.
5. Adding the ability to resize the screen.
6. Adding a more intuitive UI so that it gives the ability for a finer control over the parameters + adding a randomize button.
7. A better way to fine-tune is to use an evolutionary algorithm to select and optimize the parameters but one needs to write a fitness function for that. I currently don't know what fitness function corresponds to in the realm of this program. In our world the fitness function is competition & survival of the fittest. However, here we fine-tune and select the parameters that produces interesting patterns for us, however, the word "interesting" here is easier to say than to define!

The JavaScript code is as simple as this: 
-------------------------------------
```
  <canvas id="life" width="500" height="500"></canvas>
  <script>
    //Hunar Ahmad @ brainxyz
  m=document.getElementById("life").getContext('2d')
  draw=(x,y,c,s)=>{m.fillStyle=c; m.fillRect(x, y, s, s)}
  atoms=[]
  atom=(x,y,c)=>{return{"x":x, "y":y, "vx":0, "vy":0, "color":c}}
  random=()=>{return Math.random()*400+50}
  create=(number, color)=>{ 
  	group=[]
    for(let i=0; i < number; i++){
	    group.push(atom(random(), random(), color))
	    atoms.push(group[i]) 
	}
    return group 
  }
  rule=(atoms1, atoms2, g)=>{
  for(let i=0; i < atoms1.length; i++){
    fx = 0
    fy = 0
    for(let j=0; j < atoms2.length; j++){
      a = atoms1[i]
      b = atoms2[j]
      dx = a.x-b.x
      dy = a.y-b.y
      d = Math.sqrt(dx*dx + dy*dy)
      if(d > 0 && d < 80){
      	F = g * 1/d
        fx += (F * dx)
        fy += (F * dy)
      }
    } 
    a.vx = (a.vx + fx)*0.5
    a.vy = (a.vy + fy)*0.5
    a.x += a.vx
    a.y += a.vy
    if(a.x <= 0 || a.x >= 500){ a.vx *=-1 }
    if(a.y <= 0 || a.y >= 500){ a.vy *=-1 }
    }
  }
  yellow = create(200, "yellow")
  red = create(200, "red")
  green = create(200, "green")
  update=()=>{
    rule(green, green, -0.32)
    rule(green, red, -0.17)
    rule(green, yellow, 0.34)
    rule(red, red, -0.10)
    rule(red, green, -0.34)
    rule(yellow, yellow, 0.15)
    rule(yellow, green, -0.20)
    m.clearRect(0, 0, 500, 500)
    draw(0, 0, "black", 500)
    for(i=0; i<atoms.length; i++){ 
    draw(atoms[i].x, atoms[i].y, atoms[i].color, 5) 
    }
    requestAnimationFrame(update)
  }
  update();
  </script>

```

</br>


Related topics:
Particle Life Simulation,
Primordial Soup - Evolution,
Conway's game of life,
Cellular automata,
Self organzing patterns,

This project was inspired by: Jeffery Ventrella's Clusters http://www.ventrella.com/Clusters/
I don't have access to Ventrella's code but I guess the main difference of this project with the other particle life projects is that I didn't implement collision detection and this made simulating thousands of particles possible in realtime. Also, I added GUI controls to change the parameters in realtime this allows easy fine-tuning & exploration, hence, I was able to find some never-seen-before patterns emege form some extremely simple models of relations. The code here is probably an order of magnitude simpler than any other Artificial Life codes out there because I started this code solely as an eductaional material for non-programers and general audience to prove the point that complexity can arise from simplicity.

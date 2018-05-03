# AVI Player on SDL

I am [Joan Valiente](www.linkedin.com/in/joan-valiente-lorite-498a9b129), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).




## 1.Introduction

In this website I will explain how to implement a module to reproduce AVI files, and how it works. This modules are programmed in C++ using VisualStudio2017.
I am also using Video For Windows libraries and DirectShow to open, read and decompress AVI files; and SDL to render graphics.

I want to say that the way I have implemented this code is not absolute, or the best. There are many ways of doing it, this tutorial is just an example of how I managed to solve this problem, so if you do not want to follow this tutorial and do it your own way I encourage you to check the references links that I visited to do this.
I hope you find it useful.

## 2. Pre-rendered videos on video games

Pre-rendering is the process in which video footage is not rendered in real-time by the hardware that is outputting or playing back the video. Instead, the video is a recording of footage that was previously rendered on different equipment.
Since we are learning how to implement a video player to use it on our games projects, I think it is important to explain a little bit what their uses are, their advantatges, their disadvantages, etc.
The most common use of a video player in video games is for cut-scenes. But we can also use it for an intro animation of the studio, intermissions, credits, etc. However, pre-rendered cutscenes are mostly rejected by users nowadays, but we will talk about that later. 
One of the first games to extensively use pre-rendered graphics along with full motion video was [The 7th Guest](https://en.wikipedia.org/wiki/The_7th_Guest).
Here is a video with the cutscenes of the game.
<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/9TOkdq55zhw?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>

Since then, pre-rendered cutscenes have been used in a lot of videogames because of its narrative strength. We can see them as an introductory video to the main game, an intermission, a video that is played after a certain event, etc.
We can see some examples here.
<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/6XctypDwZ84?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>
The Witcher 3 Intro cutscene.
<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/pa1fi1gxxUw?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>
Uncharted 2. Cutscene before a level.
<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/7WtKNoeq1wM?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>
Diablo 2. Act III intermission.
<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/FbQP-H4MwQk?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>
Black Ops. Intermission cutscene.
<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/dA0kR3SLJJc?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>
Valve Intro(2012). 

### 2.1 Pre-rendered cutscenes vs real time cutscenes

Nowadays, most of the community dislikes pre-rendered cutscenes. A few years ago, their narrative strength offset their lack of interaction, but now that our computational limitations are way much smaller, users prefer real time cutscenes.

<div style="position:relative;height:0;padding-bottom:52.18%"><iframe src="https://www.youtube.com/embed/wFbWI0pwXH0?ecver=2" style="position:absolute;width:100%;height:100%;left:0" width="690" height="360" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></div>
God of War 4. Real time cutscenes. 

## 3. Our video player

How is it going to work? The theory is really simple. First, we have to read the avi file and the stream data. Then, on each loop we will take the bitmap data of a frame. With that, we will create a surface and a texture from that surface, and we will blit it on screen. Of course there are more steps to follow inbetween those, but that is a brief explanation of how it works.

### 3.1 The module

FUNCTIONS
We will have four esential functions. 
-**OpenAVI**, that opens the avi file and reads its stream data.
-**Initialize**, that calls OpenAVI with the path to the file (we will call this function whenever we want to play a video)
-**GrabAVIFrame**, that gets the frame data, makes a surface and a texture, and blit it.
-**CloseAVI**, that will free the memory we have used.
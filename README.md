# Hazmat Browser [![Licence](https://img.shields.io/badge/License-Apache%202.0-yellowgreen.svg)](https://github.com/StevenSavold/HazmatBrowser/blob/master/LICENSE)

![Hazmat Browser](/Resources/Branding/Hazmat_Icon_512x512.png)

The Hazmat Browser is a simple internet browser tool which utilizes [Google's Gumbo Parser](https://github.com/google/gumbo-parser) and the [Hazel Rendering Engine](https://github.com/TheCherno/Hazel) to display web pages.



## Getting Started
Visual Studio 2017 or 2019 is recommended, as the Hazel engine is currently only supporting Windows for now. As that changes, so will this project.

Start by cloning the repository with ```git clone --recursive https://github.com/StevenSavold/HazmatBrowser```

If the repository was cloned non-recursively previously, use ```git submodule update --init``` to clone the necessary submodules.


## The Plan
The Hazmat Browser is more of an educational tool more so then a fully featured web browser. As such the scope will ultimatly be pretty limited. I would be very happy to be able to see Hazmat load the google home page and successfully make a web search. To do that Hazmat will be implementing the following features:
  - [x] Using the Gumbo Parser output, restructure parsed HTML into a DOM Tree
  - [ ] Using the DOM Tree as input, make a Layout Engine which is capable of producing a series of Hazel draw calls which represent the boxed layout of the DOM Tree
  - [ ] Using the output of the layout engine, feed the Hazel draw calls to Hazel for rendering
  - [ ] Using (a currently undetermined Library or in-house solution) to parse and run JavaScript
  - [ ] Using (a currently undetermined Networking Library) retreive web pages to render from the internet
  - [x] Using std::filesystem load local files from the users computer to render web pages
  - [ ] Use Hazels ImGui dependency to help create the UI
  - [ ] Add anything else that would be fun or cool

If all of these are met and the project still has momentum I would love to see Hazmat go beyond its initial scope and be a much more fully featured web browser. But lets take this one step at a time.


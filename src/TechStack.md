# Tech Stack Description

This file briefly describes the tech stack for the [Norb's Nook](https://github.com/RayZh-hs/Norbs-Nook) project.

## Technology Overview

The three folders within `src/` corresponds to three types of programming genre. In detail:

- `src/api/` uses Python, in specific the [Flask](https://github.com/pallets/flask) framework. It directly uses the executable produced by the backend as a subprocess and serves on a local server.
- `src/backend/` uses C++ to provide a fast and reliable database system. A standalone console app is provided as well as the full-scale GUI application.
- `src/frontend` uses modern web technologies, in specific the [Vue](https://vuejs.org/) framework. Naive UI is used for this project. 
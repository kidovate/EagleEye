EagleEye
========

An experiment in passive memory reading and reverse engineering combined with UX. A DayZ in-browser external collaborative passive world map.

EagleEye is a DLL that can be injected into a NON-GAME related process (like your web browser).

The system works by first reading data from the game (player pos, loot tables,
other player names and positions) and then streaming this data in realtime
to a remote server, which hosts a Node.JS powered map to visualize the data.

The map can be viewed on other computers as well. If you share the URL to the map
your friends can use the map on the same server as well.
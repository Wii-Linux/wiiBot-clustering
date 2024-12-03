# Wii Bot (Clustering edition)
Discord bot written in C with Concord.
Currently just a basic PoC, but eventually will have a pairing backend node that will be deployed on user's Wii's, who would like to run a node.
It will also expose an API that will allow monitoring the status of the overal cluster, and nodes.

## Central server (frontend) details
- Exposes API over TCP that can be used by a web server or similar to display human-readable info about the cluster
- Has a Discord bot that allows users to interface with the cluster
- Hosts the socket with which nodes will communicate with

## Node server (backend) details
- A node service that will run on each individual Wii
- Communicates over unencrypted TCP, but intended to be used for an encrypted VPN (e.g. Wireguard)
- Central server reaches out to the Wiis for communication, and repeatedly sends a keepalive message over the TCP socket to ensure the connection stays up
- Backend will give up and kill any running command, should it no longer be able to reach the central server
- Central server will remove node from the list of usable nodes should it not be able to reach it
- All using centralized network storage over NFS, for one canonical system, shared across all nodes.

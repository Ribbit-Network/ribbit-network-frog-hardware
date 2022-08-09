FROM node:latest

WORKDIR /source
COPY . .

# build frontend
WORKDIR /source/dashboard
RUN npm i
RUN npm run build

# build server
WORKDIR /source
RUN npm i
RUN npm run build

EXPOSE 3000

CMD ["npm", "start"]

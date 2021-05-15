var express = require('express');
var cors = require('cors');

const dotenv = require('dotenv')
const routes = require('./routes/routes')

dotenv.config()
const app = express()

app.use(express.json())
app.use(cors())
app.use('/app', routes)
app.listen(4000, () => console.log("Server running"))

module.exports = app;

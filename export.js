const axios = require("axios");
const XLSX = require("xlsx");
const artifact = require('@actions/artifact');
const core = require('@actions/core');

const token = process.env.GITHUB_TOKEN;
const headers = {
  "Authorization": `Token ${token}`
};
const repoName = "nodeaffinity";
const repoOwner = "saquibkhan";
axios.get(`https://api.github.com/repos/${repoOwner}/${repoName}/issues`, { headers })
 .then(response => {
 console.log("************** response data *******************");
  console.log(response.data);
  
  const filteredData = response.data.filter(obj => obj.title && obj.id && obj.created_at);

 console.log("************** filteredData *******************");
  console.log(filteredData);

const desiredData = filteredData.map(obj => ({
  "Issue Name": obj.title,
  "ID": obj.id,
  "Created Date": obj.created_at
}));

 console.log("************** desiredData *******************");
  console.log(desiredData);

const workbook = XLSX.utils.book_new();
const worksheet = XLSX.utils.json_to_sheet(desiredData);
  
  
  //const workbook = XLSX.utils.json_to_sheet(desiredData);
   console.log("************** worksheet *******************");
  console.log(worksheet);
//XLSX.writeFile(XLSX.utils.book_new([workbook]), "issues.xlsx");});

XLSX.utils.book_append_sheet(workbook, worksheet, 'Sheet1');
XLSX.writeFile(workbook, 'issues.xls');});


// Upload the xlsx file to GitHub's artifact storage

const artifactClient = artifact.create()

const pathToXlsxFile = "issues.xlsx";
const artifactName = "issues";
const files = [
    'issues.xls'
];

const rootDirectory = '.' // Also possible to use __dirname
const options = {
    continueOnError: false
}

artifactClient.uploadArtifact(artifactName, files, rootDirectory, options);

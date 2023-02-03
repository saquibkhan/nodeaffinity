const axios = require("axios");
const XLSX = require("xlsx");
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
const artifactUploader = require('@actions/artifact').uploader;
const core = require('@actions/core');

const pathToXlsxFile = "issues.xlsx";
const artifactName = "issues";

artifactUploader.uploadArtifact(artifactName, [pathToXlsxFile], path.resolve("./"))
  .then(() => {
    console.log(`Artifact ${artifactName} uploaded`);
    core.setOutput("artifactName", artifactName);
  })
  .catch(error => {
    console.error(error);
    core.setFailed(error.message);
  });

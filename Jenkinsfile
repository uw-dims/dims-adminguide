pipeline {
  agent any

  stages {
    stage('Build') {
      steps {
        sh '(cd docs; make latexpdf)'
      }
    }
    stage('Deploy') {
      steps {
        sh '(cd docs/build/latexpdf; cp *.pdf /tmp)'
      }
    }
  }
}

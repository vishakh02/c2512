mport chromadb
from chromadb.utils import embedding_functions
import ollama
import json
import logging
 
# Configure logging
logging.basicConfig(level=logging.ERROR, format='%(asctime)s - %(levelname)s - %(message)s')
 
# Load settings from JSON file
try:
    with open("settings.json", "r") as f:
        settings = json.load(f)
except Exception as e:
    logging.error(f"Error loading settings.json: {e}")
    settings = {"model_name": "mistral"}
 
class ChromaDBHandler:
    """Class to handle ChromaDB operations."""
    def __init__(self, db_path="./chroma_db", collection_name="rag_collection"):
        try:
            self.client = chromadb.PersistentClient(path=db_path)
            self.collection = self.get_or_create_collection(collection_name)
        except Exception as e:
            logging.error(f"Error initializing ChromaDBHandler: {e}")
            self.collection = None
    def get_or_create_collection(self, collection_name):
        """Retrieve or create a ChromaDB collection."""
        try:
            return self.client.get_or_create_collection(name=collection_name, embedding_function=embedding_functions.DefaultEmbeddingFunction())
        except Exception as e:
            logging.error(f"Error retrieving/creating collection: {e}")
            return None
    def add_string(self, string_id, text):
        """Add a string to the ChromaDB collection."""
        if not self.collection:
            logging.error("ChromaDB collection not initialized.")
            return False
        try:
            self.collection.add(ids=[string_id], documents=[text])
            print(f"Added: {string_id} -> {text}")
            return True
        except Exception as e:
            logging.error(f"Error adding string: {e}")
            return False
    def query(self, query_text, top_k=3):
        """Query ChromaDB for the most relevant strings with ranking."""
        if not self.collection:
            logging.error("ChromaDB collection not initialized.")
            return []
        try:
            results = self.collection.query(query_texts=[query_text], n_results=top_k)
            ranked_results = []
            if results.get("documents") and results["documents"][0]:
                for doc, score in zip(results["documents"][0], results["distances"][0]):
                    ranked_results.append((doc, score))
                ranked_results.sort(key=lambda x: x[1])  # Sort by distance (lower is better)
            return ranked_results
        except Exception as e:
            logging.error(f"Error querying database: {e}")
            return []
 
class LLMHandler:
    """Class to interact with the LLM model."""
    def __init__(self, model_name=settings.get("model_name", "mistral")):
        self.model_name = model_name
    def query_llm(self, query_text):
        """Use an LLM to process user queries."""
        try:
            response = ollama.chat(model=self.model_name, messages=[{"role": "user", "content": query_text}])
            return response.get("message", {}).get("content", "")
        except Exception as e:
            logging.error(f"Error querying LLM: {e}")
            return ""
 
class RAGSystem:
    """Class to handle the overall RAG system combining ChromaDB and LLM."""
    def __init__(self):
        self.db_handler = ChromaDBHandler()
        self.llm_handler = LLMHandler()
    def ask_question(self, query):
        """Process user question, retrieve and rank relevant information."""
        llm_response = self.llm_handler.query_llm(query)
        if not llm_response:
            logging.error("LLM response is empty.")
            return False
        results = self.db_handler.query(llm_response)
        return results if results else False
    def add_string(self, string_id, text):
        """Add a string to the database through RAGSystem."""
        return self.db_handler.add_string(string_id, text)
 
def main():
    rag_system = RAGSystem()
    # Example: Adding strings to ChromaDB
    print(rag_system.add_string("1", "Artificial Intelligence is revolutionizing the world."))
    print(rag_system.add_string("2", "Machine learning is a subset of AI."))
    print(rag_system.add_string("3", "ChromaDB is a powerful vector database for RAG applications."))
    # Example: Querying with LLM assistance
    query = "What is AI?"
    results = rag_system.ask_question(query)
    if results:
        print("Ranked results:")
        for doc, score in results:
            print(f"Score: {score:.4f} -> {doc}")
    else:
        print("No results found.")
 
if __name__ == "__main__":
    main()

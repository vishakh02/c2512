import json
import os
import re
from langchain.ollama import OllamaEmbeddings, OllamaLLM
import chromadb
from sentence_transformers import CrossEncoder
 
chromadb_id = "doc00"
 
System_prompt = """
You are an AI assistant designed to provide precise and detailed responses based strictly on the given context. Your objective is to analyze the provided information thoroughly and generate a well-structured, comprehensive answer to the user's question.
 
The context will be provided as "Context:" 
The user’s question will be provided as "Question:"  
 
To generate an effective response:
 
1. Carefully analyze the context, extracting key details relevant to the question.  
2. Structure your response logically to ensure clarity and coherence.  
3. Formulate a detailed answer that directly addresses the question, relying only on the provided context.  
4. Ensure completeness by covering all relevant aspects mentioned in the context.  
5. If the context lacks sufficient information to fully answer the question, explicitly state this.
"""
 
def load_config():
    try:
        with open("settings.json", "r") as f:
            config = json.load(f)
        return config
    except Exception as e:
        print(f"Error loading configuration: {e}")
        return None
 
class ChromaDBEmbeddingFunction:
    def __init__(self, langchain_embeddings):
        self.langchain_embeddings = langchain_embeddings
 
    def __call__(self, input):
        if isinstance(input, str):
            input = [input]
        return self.langchain_embeddings.embed_documents(input)
 
def initialize_system(config):
    try:
        llm_model = config["llm_model"]
        embedding_model = config["embedding_model"]
        chroma_client = chromadb.PersistentClient(path=os.path.join(os.getcwd(), "chroma_db"))
        embedding_function = ChromaDBEmbeddingFunction(
            OllamaEmbeddings(model=embedding_model, base_url=config["ollama_base_url"])
        )
        collection_name = "rag_collection_demo_1"
        collection = chroma_client.get_or_create_collection(
            name=collection_name,
            metadata={"description": "A collection for RAG with Ollama Demo1"},
            embedding_function=embedding_function
        )
        print("System initialized successfully.")
        return collection, llm_model
    except Exception as e:
        print(f"Error initializing system: {e}")
        return None, None
 
def add_documents_to_chroma(collection, documents):
    try:
        global chromadb_id
        match = re.search(r'(\d+)$', chromadb_id)
        if match:
            num = int(match.group(1)) + 1
            chromadb_id = chromadb_id[:-len(match.group(1))] + str(num)
        collection.add(documents=documents, ids=[chromadb_id])
        print("Documents added to ChromaDB successfully.")
        return {"status": "success", "message": "Documents added."}
    except Exception as e:
        print(f"Error adding documents to ChromaDB: {e}")
        return {"status": "error", "message": str(e)}
 
def retrieve_documents_from_chroma(collection, query_text, n_results=5):
    try:
        results = collection.query(query_texts=[query_text], n_results=n_results)
        if results["documents"]:
            print("Documents retrieved successfully.")
            return {"status": "success", "documents": results["documents"], "metadatas": results["metadatas"]}
        else:
            print("No relevant documents found.")
            return {"status": "error", "message": "No relevant documents found."}
    except Exception as e:
        print(f"Error querying ChromaDB: {e}")
        return {"status": "error", "message": str(e)}
 
def similarity_search_reranking(retrieved_docs, query_text):
    try:
        if not retrieved_docs:
            return None
        model = CrossEncoder('cross-encoder/ms-marco-MiniLM-L-6-v2')
        scores = model.predict([[query_text, doc] for doc in retrieved_docs])
        ranked_docs = sorted(zip(retrieved_docs, scores), key=lambda x: x[1], reverse=True)
        best_doc = ranked_docs[0][0] if ranked_docs else None
        print("Similarity search and reranking completed.")
        return best_doc
    except Exception as e:
        print(f"Error in similarity search and reranking: {e}")
        return None
 
def query_ollama(llm_model, prompt):
    try:
        llm = OllamaLLM(model=llm_model)
        response = llm.invoke(f"{System_prompt}\nContext: {prompt}\n\nQuestion: {prompt}\nAnswer:")
        print("Query sent to Ollama successfully.")
        return {"status": "success", "response": response}
    except Exception as e:
        print(f"Error querying Ollama: {e}")
        return {"status": "error", "message": str(e)}
 
def generate_response_using_rag(collection, llm_model, query_text):
    try:
        retrieval_result = retrieve_documents_from_chroma(collection, query_text)
        if retrieval_result["status"] == "error":
            return retrieval_result
        best_doc = similarity_search_reranking(retrieval_result["documents"], query_text)
        if not best_doc:
            return {"status": "error", "message": "No relevant document found after reranking."}
        augmented_prompt = f"Context: {best_doc}\n\nQuestion: {query_text}\nAnswer:"
        response_result = query_ollama(llm_model, augmented_prompt)
        if response_result["status"] == "error":
            return response_result
        return {"status": "success", "response": response_result["response"]}
    except Exception as e:
        print(f"Error generating response: {e}")
        return {"status": "error", "message": str(e)}
 
def main():
    try:
        config = load_config()
        if not config:
            return {"status": "error", "message": "Failed to load configuration."}
        collection, llm_model = initialize_system(config)
        if not collection or not llm_model:
            return {"status": "error", "message": "System initialization failed."}
        documents = [
            "Josh, Sooraj, Nimisha, Venkatesh, Vishakh are interns.",
            "Python is a programming language that lets you work quickly and integrate systems more effectively.",
            "ChromaDB is a vector database designed for AI applications."
        ]
        add_result = add_documents_to_chroma(collection, documents)
        if add_result["status"] == "error":
            return add_result
        query = input("Input your query: ")
        response = generate_response_using_rag(collection, llm_model, query)
        print("Response:")
        print(response)
        return response
    except Exception as e:
        print(f"Error in main execution: {e}")
        return {"status": "error", "message": str(e)}
 
if __name__ == "__main__":
    main()
